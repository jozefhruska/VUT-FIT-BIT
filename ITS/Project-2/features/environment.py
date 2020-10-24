#!/usr/bin/env python3

from selenium import webdriver
from selenium.webdriver.common.desired_capabilities import DesiredCapabilities

def before_all(context):
	# Set the default preferences
	preferences = {
		'browserName': 'chrome',
		'marionette': 'true',
		'javascriptEnabled': 'true'
	}

	# Set the remote
	context.browser = webdriver.Remote(
		command_executor = 'http://mys01.fit.vutbr.cz:4444/wd/hub',
		desired_capabilities = preferences
	)

	# Timeout
	context.browser.implicitly_wait(15)

	# Base URL
	context.base_url = "http://mys01.fit.vutbr.cz:8024/"


def after_all(context):
	context.browser.quit()
