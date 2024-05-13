import requests

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


def write_coordinates_to_file(coordinates, output_filename):
    with open(output_filename, 'w') as file:
        for coord in coordinates:
            file.write(f"{coord[0]}, {coord[1]}\n")
        print(f"Coordinates written to {output_filename}")


def read_addresses_from_file(filename):
    with open(filename, 'r') as file:
        addresses = [line.strip() for line in file]
    return addresses


# Example usage
if __name__ == '__main__':
    addresses = read_addresses_from_file('addressD.txt')
    coordinates = geocode_address(addresses)
    if coordinates:
        output_filename = 'command_cor.txt'
        write_coordinates_to_file(coordinates, output_filename)
    else:
        print("Addresses could not be geocoded.")
