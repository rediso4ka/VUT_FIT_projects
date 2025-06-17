#!/bin/sh
python3 get_urls.py > url_test.txt
head url_test.txt | python3 get_product_parameters.py