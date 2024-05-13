#include "gas.h"
#include <cmath>
#include <fstream>
#include <sstream>
#include <string>

#define earthRadiusKm 6371.0                // Earth's radius in kilometers.
#define deg2rad(deg) ((deg) * (M_PI / 180)) // converts degrees to radians

gas::gas()
{
}

// Function to calculate distance between two points using Haversine formula
double gas::haversine(double lat1, double lon1, double lat2, double lon2)
{
    double dLat = deg2rad(lat2 - lat1);
    double dLon = deg2rad(lon2 - lon1);

    lat1 = deg2rad(lat1);
    lat2 = deg2rad(lat2);

    double a = sin(dLat / 2) * sin(dLat / 2) +
               sin(dLon / 2) * sin(dLon / 2) * cos(lat1) * cos(lat2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));
    return earthRadiusKm * c;
}

double gas::calc_distance()
{
    std::ifstream hq_file("C:\\Users\\everp\\Documents\\GitHub\\2a23-smart_delivery\\gas\\hq_cor.txt");
    std::ifstream address_file("C:\\Users\\everp\\Documents\\GitHub\\2a23-smart_delivery\\gas\\command_cor.txt");

    double hq_lat, hq_lon;
    char ch; // to discard the comma
    hq_file >> hq_lat >> ch >> hq_lon;

    double sum_distance = 0.0;
    std::string line;
    while (std::getline(address_file, line))
    {
        std::istringstream iss(line);
        double lat, lon;
        if (!(iss >> lat >> ch >> lon)) // read and discard the comma
        {
            break;
        } // error

        double distance = haversine(hq_lat, hq_lon, lat, lon);
        sum_distance += distance;
    }

    return sum_distance;
}

double gas::fuel_consumption()
{
    double distance = calc_distance(); // distance in km
    double fuelConsumptionRate = 33.0; // fuel consumption in L/100km for a semi-truck

    double litres_needed = (distance / 100) * fuelConsumptionRate; // calculate litres needed

    return litres_needed;
}

float gas::calc_cost()
{
    double litres_needed = fuel_consumption(); // get litres needed from fuel_consumption function

    std::ifstream gas_price_file("C:\\Users\\everp\\Documents\\GitHub\\2a23-smart_delivery\\gas\\gas_price.txt");
    double gas_price_per_litre;
    gas_price_file >> gas_price_per_litre;

    float cost = litres_needed * gas_price_per_litre; // calculate cost

    return cost;
}

float gas::calc_CO2()
{
    double litres_needed = fuel_consumption(); // get litres needed from fuel_consumption function

    double CO2_emission_factor = 2.68; // CO2 emission factor for diesel fuel in kg CO2 per litre

    float CO2_emissions = litres_needed * CO2_emission_factor; // calculate CO2 emissions

    return CO2_emissions; // in kg
}
