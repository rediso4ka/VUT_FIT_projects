from urllib.request import urlopen, Request
from urllib.error import HTTPError, URLError
from bs4 import BeautifulSoup
from sys import stderr

def get_page(url):
    try:
        request = Request(url)
        request.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3')

        response = urlopen(request)
        if response.status != 200:
            print(f"Failed to access page. Status code: {response.status}", file=stderr)
            exit(1)

        page = response.read().decode("utf-8")

    except (HTTPError, URLError) as error:
        print(f"Error fetching {url}: {error}", file=stderr)
        exit(1)

    return page

def get_product_links(url, links_number):
    links = set()
    page_number = 1

    while len(links) < links_number:
        page_url = f"{url}?page={page_number}" if page_number > 1 else url
        page = get_page(page_url)

        soup = BeautifulSoup(page, "html.parser")
        products = soup.select('#products-list #js-product-list .product_desc .product_name')
        
        if not products:
            print(f"No products found.", file=stderr)
            exit(1)

        for product in products:
            links.add(product['href'])

        page_number += 1

    return links

if __name__ == "__main__":
    url = 'https://tradingshenzhen.com/en/smartphones'
    links_number = 150

    product_links = get_product_links(url, links_number)
    for link in product_links:
        print(link)