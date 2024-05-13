import requests

MODE = 2  # 1 auto , 2 manual


def get_diesel_price():
    headers = {
        'Authorization': 'Token ed73c8e95d9389bb1b00872675125fdc',
    }
    response = requests.get(
        'https://api.oilpriceapi.com/v1/prices/latest', headers=headers)
    data = response.json()
    # print(data)  # print out the data to debug
    price_per_barrel_usd = data['data']['price']
    price_per_liter_usd = price_per_barrel_usd / 159
    return price_per_liter_usd


def get_exchange_rate():
    response = requests.get(
        'https://v6.exchangerate-api.com/v6/b9d087308aa9ba1acc82a331/latest/USD')
    data = response.json()
    # print(data)  # print out the data to debug
    usd_to_tnd_rate = data['conversion_rates']['TND']
    return usd_to_tnd_rate


def get_diesel_price_in_tnd():
    if MODE == 2:
        return 1.8
    diesel_price_usd = get_diesel_price()
    exchange_rate = get_exchange_rate()
    diesel_price_tnd = diesel_price_usd * exchange_rate

    return round(diesel_price_tnd, 3)


with open('gas_price.txt', 'w') as f:
    f.write(str(get_diesel_price_in_tnd()))
