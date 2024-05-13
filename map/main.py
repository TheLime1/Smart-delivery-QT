import requests
from PIL import Image
from io import BytesIO

BING_MAP_API_KEY = 'AlcgBfu6QdRzgnAhzBPMYMmLvDQdFgdu9MhLGJRVLj_Kp9bVf_Ar-UFHFaXk9hl8'


def geocode_address(addresses):
    api_key = BING_MAP_API_KEY
    base_url = 'http://dev.virtualearth.net/REST/v1/Locations'
    coordinates = []
    for address in addresses:
        params = {'q': address, 'key': api_key}
        response = requests.get(base_url, params=params)
        data = response.json()
        if data['resourceSets'] and data['resourceSets'][0]['resources']:
            location = data['resourceSets'][0]['resources'][0]['point']['coordinates']
            print(f"Geocoded {address} to {location}")
            coordinates.append((location[0], location[1]))  # lat, lng
        else:
            print(f"Failed to geocode {address}")
    return coordinates


def generate_map_with_pushpins(coordinates, output_filename):
    api_key = BING_MAP_API_KEY
    base_url = 'http://dev.virtualearth.net/REST/v1/Imagery/Map/Road'

    pushpins = []  # Create a list to store pushpin strings
    for i, coord in enumerate(coordinates):
        lat, lng = coord
        if i == 0:
            pushpin = f'?pushpin={lat},{lng}'  # ! First pushpin with "?"
        else:
            pushpin = f'&pushpin={lat},{lng}'  # ! Subsequent pushpins with "&"
        pushpins.append(pushpin)

    # Join the pushpins and add API key to the URL
    pushpin_url = ''.join(pushpins)
    base_url += pushpin_url + f'&key={api_key}'

    response = requests.get(base_url)
    if response.status_code == 200:
        img = Image.open(BytesIO(response.content))
        img.save(output_filename)
        print(f"Map image with pushpins saved as {output_filename}")
    else:
        print(f"Failed to generate map image: {response.content}")


def read_addresses_from_file(filename):
    with open(filename, 'r') as file:
        addresses = [line.strip() for line in file]
    return addresses


# Example usage
if __name__ == '__main__':
    addresses = read_addresses_from_file('address.txt')
    coordinates = geocode_address(addresses)
    if coordinates:
        output_filename = 'map.png'
        generate_map_with_pushpins(coordinates, output_filename)
    else:
        print("Addresses could not be geocoded.")
