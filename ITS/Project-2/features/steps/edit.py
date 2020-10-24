from behave import *
from selenium.webdriver.common.keys import Keys
from selenium.webdriver.common.action_chains import ActionChains

import urllib.parse as urlparse
import re

def getToken(browser):
	parsed = urlparse.urlparse(browser.current_url)
	return urlparse.parse_qs(parsed.query)['token'][0]

category = None
categoryCount = 0
imagesCount = 0

@given(u'a web browser is on a product edit page')
def step_impl(context):
	context.browser.get("http://mys01.fit.vutbr.cz:8024/admin/index.php?route=catalog/product/edit&token=%s&product_id=41" % getToken(context.browser))
	assert(u'Edit Product' == context.browser.find_element_by_css_selector("#content h3.panel-title").text)

@given(u'the "General" tab is selected in tab menu')
def step_impl(context):
	context.browser.find_element_by_css_selector("#content .nav-tabs li:nth-child(1) a").click()
	assert(u'General' == context.browser.find_element_by_css_selector("#content .nav-tabs li.active a").text)

@given(u'the user adds "42" to the "Product name" input')
def step_impl(context):
	nameInput  = context.browser.find_element_by_css_selector("#input-name1")
	nameInput.send_keys("42")

@when(u'the user clicks on the "Save" button')
def step_impl(context):
	context.browser.find_element_by_css_selector("#content .page-header button[type='submit']").click()

@then(u'product edit page is reloaded')
def step_impl(context):
	assert(u'Product List' == context.browser.find_element_by_css_selector("#content h3.panel-title").text)

@then(u'the "Product name" value is "iMac42"')
def step_impl(context):
	context.browser.get("http://mys01.fit.vutbr.cz:8024/admin/index.php?route=catalog/product/edit&token=%s&product_id=41" % getToken(context.browser))
	assert(u'iMac42' == context.browser.find_element_by_css_selector("#input-name1").get_attribute("value"))

@given(u'the "Data" tab is selected in tab menu')
def step_impl(context):
	context.browser.find_element_by_css_selector("#content .nav-tabs li:nth-child(2) a").click()
	assert(u'Data' == context.browser.find_element_by_css_selector("#content .nav-tabs li.active a").text)

@when(u'the user hovers on SKU tooltip icon')
def step_impl(context):
	ActionChains(context.browser).move_to_element(context.browser.find_element_by_css_selector("#tab-data > div:nth-child(2) > label > span")).perform()

@then(u'the Stock Keeping Unit is shown')
def step_impl(context):
	assert(u'Stock Keeping Unit' == context.browser.find_element_by_css_selector("#tab-data > div:nth-child(2) > label > span").get_attribute('data-original-title'))

@when(u'the user hovers on UPC tooltip icon')
def step_impl(context):
	ActionChains(context.browser).move_to_element(context.browser.find_element_by_css_selector("#tab-data > div:nth-child(3) > label > span")).perform()

@then(u'the Universal Product Code is shown')
def step_impl(context):
	assert(u'Universal Product Code' == context.browser.find_element_by_css_selector("#tab-data > div:nth-child(3) > label > span").get_attribute('data-original-title'))

@when(u'the user hovers on EAN tooltip icon')
def step_impl(context):
	ActionChains(context.browser).move_to_element(context.browser.find_element_by_css_selector("#tab-data > div:nth-child(4) > label > span")).perform()

@then(u'the European Article Number is shown')
def step_impl(context):
	assert(u'European Article Number' == context.browser.find_element_by_css_selector("#tab-data > div:nth-child(4) > label > span").get_attribute('data-original-title'))

@when(u'the user hovers on JAN tooltip icon')
def step_impl(context):
	ActionChains(context.browser).move_to_element(context.browser.find_element_by_css_selector("#tab-data > div:nth-child(5) > label > span")).perform()

@then(u'the Japanese Article Number is shown')
def step_impl(context):
	assert(u'Japanese Article Number' == context.browser.find_element_by_css_selector("#tab-data > div:nth-child(5) > label > span").get_attribute('data-original-title'))

@when(u'the user hovers on ISBN tooltip icon')
def step_impl(context):
	ActionChains(context.browser).move_to_element(context.browser.find_element_by_css_selector("#tab-data > div:nth-child(6) > label > span")).perform()

@then(u'the International Standard Book Number is shown')
def step_impl(context):
	assert(u'International Standard Book Number' == context.browser.find_element_by_css_selector("#tab-data > div:nth-child(6) > label > span").get_attribute('data-original-title'))

@when(u'the user hovers on MPN tooltip icon')
def step_impl(context):
	ActionChains(context.browser).move_to_element(context.browser.find_element_by_css_selector("#tab-data > div:nth-child(7) > label > span")).perform()

@then(u'the Manufacturer Part Number is shown')
def step_impl(context):
	assert(u'Manufacturer Part Number' == context.browser.find_element_by_css_selector("#tab-data > div:nth-child(7) > label > span").get_attribute('data-original-title'))

@when(u'the user clicks on calendar icon for "Date Available" input')
def step_impl(context):
	context.browser.find_element_by_css_selector("#tab-data > div:nth-child(17) > div > div > span > button").click()

@then(u'a calendar selector is shown below the input')
def step_impl(context):
	classAttribute = context.browser.find_element_by_css_selector("body > div.bootstrap-datetimepicker-widget.dropdown-menu").get_attribute("class")
	assert(re.search("picker-open", classAttribute) != False)

@given(u'an empty value is in "Model" input')
def step_impl(context):
	modelInput = context.browser.find_element_by_css_selector("#input-model")
	modelInput.clear()
	assert(modelInput.text == '')

@then(u'a warning is displayed below the page title')
def step_impl(context):
	context.browser.find_element_by_css_selector("#content > div.container-fluid > div.alert.alert-danger")

@then(u'an explanation is displayed below the "Model" input')
def step_impl(context):
	context.browser.find_element_by_css_selector("#content .nav-tabs li:nth-child(2) a").click()
	assert(context.browser.find_element_by_css_selector("#tab-data > div.form-group.required.has-error > div > div").text != None)

@given(u'the "Links" tab is selected in tab menu')
def step_impl(context):
	context.browser.find_element_by_css_selector("#content .nav-tabs li:nth-child(3) a").click()
	assert(u'Links' == context.browser.find_element_by_css_selector("#content .nav-tabs li.active a").text)

@given(u'products is in at least one category')
def step_impl(context):
	childCategories = context.browser.find_elements_by_css_selector("#product-category div")
	categoryCount = len(childCategories)
	category = context.browser.find_element_by_css_selector("#product-category div:first-child")
	assert(categoryCount > 0)

@when(u'the user clicks on the remove icon next to a category name')
def step_impl(context):
	context.browser.find_element_by_css_selector("#product-category > div:first-child > i.fa-minus-circle").click()

@then(u'category dissapears from the list')
def step_impl(context):
	childCategories = context.browser.find_elements_by_css_selector("#product-category div")
	assert(len(childCategories) != categoryCount)

@then(u'the product is no longer in this category')
def step_impl(context):
	childCategories = context.browser.find_elements_by_css_selector("#product-category > div")
	for childCategory in childCategories:
		assert(childCategory.text != category.text)

@given(u'the "Image" tab is selected in tab menu')
def step_impl(context):
	context.browser.find_element_by_css_selector("#content .nav-tabs li:nth-child(9) a").click()
	assert(u'Image' == context.browser.find_element_by_css_selector("#content .nav-tabs li.active a").text)

@given(u'products has at least one image')
def step_impl(context):
	images = context.browser.find_elements_by_css_selector("#images > tbody > tr")
	imagesCount = len(images)
	assert(imagesCount > 0)

@when(u'the user clicks on the remove icon in the last table column')
def step_impl(context):
	context.browser.find_element_by_css_selector("#images > tbody > tr:nth-child(1) > td:nth-child(3) > button").click()

@then(u'image dissapears from the list')
def step_impl(context):
	images = context.browser.find_elements_by_css_selector("#images > tbody > tr")
	assert(len(images) != imagesCount)