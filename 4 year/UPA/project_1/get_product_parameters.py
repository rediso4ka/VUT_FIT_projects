from urllib.request import urlopen, Request
from urllib.error import HTTPError, URLError
from bs4 import BeautifulSoup
from sys import stdin

def get_product(url):
    try:
        request = Request(url)
        request.add_header('User-Agent', 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/58.0.3029.110 Safari/537.3')

        response = urlopen(request)
        if response.status != 200:
            print(f"Failed to access page. Status code: {response.status}")
            exit(1)

        product = response.read().decode("utf-8")

    except (HTTPError, URLError) as error:
        print(f"Error fetching {url}: {error}")
        exit(1)

    return product

def find(soup, tag, attributes):
    found = soup.find(tag, attrs=attributes)
    if not found:
        return "[NOT FOUND]"

    return found.text.strip()

def find_parameter(soup, matched_text):
    found = soup.find_all('dt', attrs={"class": "name"})

    for parameter in found:
        if parameter.text.strip() == matched_text:
            return parameter.find_next_sibling('dd', class_='value').text.strip()

    return "[NOT FOUND]"

def get_parameters(product):
    soup = BeautifulSoup(product, "html.parser")
    parameters = list()

    # Name tag
    tag = "h1"; attributes = {"class": "h1 namne_details", "itemprop": "name"}
    parameters.append(find(soup, tag, attributes))

    # Price tag
    tag = "span"; attributes = {"itemprop": "price"}
    parameters.append(find(soup, tag, attributes))

    # Other parameters
    parameters.append(find_parameter(soup, "Internal Storage"))
    parameters.append(find_parameter(soup, "RAM"))
    parameters.append(find_parameter(soup, "Front Camera"))
    parameters.append(find_parameter(soup, "Back Camera"))
    parameters.append(find_parameter(soup, "Battery"))
    parameters.append(find_parameter(soup, "Wireless Charging"))

    return parameters

if __name__ == "__main__":
    urls = stdin.read().splitlines()
    for url in urls:
        product = get_product(url)
        parameters = get_parameters(product)
        print(url, *parameters, sep="\t")