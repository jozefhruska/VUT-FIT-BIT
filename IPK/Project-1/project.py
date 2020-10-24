#!/usr/bin/python
# -*- coding: UTF-8 -*-

# Modules
import sys
import argparse
import socket
import json
import urllib.parse

# Objects
from argparse import RawTextHelpFormatter

# Create basic usage documentation
parser = argparse.ArgumentParser(description='OpenWeatherMap API:\nProject created for the IPK course by Jozef Hruska(xhrusk25)', formatter_class=RawTextHelpFormatter)

parser.add_argument("API_KEY", type=str, help="OpenWeatherMap API key")
parser.add_argument("CITY", type=str, help="City for which to show weather report")

## Make the parameters 'URL friendly'
args = parser.parse_args()
args.CITY = urllib.parse.quote_plus(args.CITY)
args.API_KEY = urllib.parse.quote_plus(args.API_KEY)

# Create socket
MAIN_SOCKET = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

try:
	# Define global variables
	API_HOST = "api.openweathermap.org"
	API_URL = "/data/2.5/weather?q=%s&appid=%s&units=metric" % (args.CITY, args.API_KEY)
	REQUEST = "GET %s HTTP/1.1\r\nHost: %s\r\n\r\n" % (API_URL, API_HOST)

	REQUEST_BYTES = str.encode(REQUEST)

	# Socket connection
	MAIN_SOCKET.connect((API_HOST, 80))
	MAIN_SOCKET.sendall(REQUEST_BYTES)
	data = MAIN_SOCKET.recv(4096).decode().split("\r\n\r\n")[-1]

	# Convert & print out data
	parsedData = json.loads(data)

	# Handle OpenWeatherMap API errors
	if parsedData['cod'] != 200:
		print('OpenWeatherMap API Error: ' + parsedData['message'])
		exit(2)

	# Print out data
	if "name" in parsedData:
		print("%s" % parsedData["name"])
	else:
		print("Error: Server didn't return City name.")

	if (
			"weather" in parsedData
			and 0 in parsedData["weather"]
			and "description" in parsedData["weather"][0]
		):
		print("%s" % parsedData["weather"][0]["description"])

	if "main" in parsedData:
		if "temp" in parsedData["main"]:
			print("temp: %s%sC" % (parsedData["main"]["temp"], u"\u00b0"))

		if "humidity" in parsedData["main"]:
			print("humidity: %s%s" % (parsedData["main"]["humidity"], u"\u0025"))

		if "pressure" in parsedData["main"]:
			print("pressure: %s hPa" % parsedData["main"]["pressure"])

	if "wind" in parsedData:
		if "speed" in parsedData["wind"]:
			print("wind-speed: %skm/s" % parsedData["wind"]["speed"])
		else:
			print("wind-speed: 0km/s")

		if "deg" in parsedData["wind"]:
			print("wind-deg: %s" % parsedData["wind"]["deg"])
		else:
			print("wind-deg: -")
	else:
		print("wind-speed: 0km/s")
		print("wind-deg: -")

	MAIN_SOCKET.close()
	sys.exit(0)
except socket.error as e:
	MAIN_SOCKET.close()
	print("Socket error: %s" % e)
	sys.exit(1)
except socket.timeout:
	MAIN_SOCKET.close()
	print("Socket error: Connection timeout.")
	sys.exit(1)