from behave import *

# Get page title shorthand
def getPageTitle(context):
	return context.browser.find_element_by_xpath("//div[@id='content']/div/div/h1").text


@given(u'an e-shop is selling the iPhone product')
def step_impl(context):
	context.browser.get("http://mys01.fit.vutbr.cz:8024/index.php?route=product/product&product_id=40")
	notFoundTitle = context.browser.find_element_by_xpath("//div[@id='content']/div/div[2]/h1").text
	assert(u'iPhone' == notFoundTitle)

@given(u'the user is signed in')
def step_impl(context):
	context.browser.get("http://mys01.fit.vutbr.cz:8024/admin/")
	context.browser.find_element_by_id("input-username").send_keys("admin")
	context.browser.find_element_by_id("input-password").send_keys("admin")
	context.browser.find_element_by_css_selector("button.btn-primary").click()

	pageTitle = getPageTitle(context)
	assert(u'Dashboard' == pageTitle)

@given(u'a web browser is on the product catalog page')
def step_impl(context):
	context.browser.find_element_by_id("catalog").click()
	context.browser.find_element_by_xpath("//li[@id='catalog']/ul/li[2]").click()

	pageTitle = getPageTitle(context)
	assert(u'Products' == pageTitle)

@given(u'the product name input is focused')
def step_impl(context):
	context.browser.find_element_by_id("input-name").click()

@when(u'the phrase "ipho" is entered')
def step_impl(context):
	context.browser.find_element_by_id("input-name").send_keys("ipho")

@then(u'the dropdown menu is visible')
def step_impl(context):
	firstItem = context.browser.find_element_by_xpath("//div[@id='content']/div[2]/div/div[2]/div/div/div/div/ul/li/a").text
	assert(u'iPhone' == firstItem)

@when(u'the user clicks on the status input')
def step_impl(context):
	context.browser.find_element_by_id("input-status").click()

@then(u'status options in dropdown are shown')
def step_impl(context):
	defaultOption = context.browser.find_element_by_css_selector("#input-status > option:nth-child(1)").text
	enabledOption = context.browser.find_element_by_css_selector("#input-status > option:nth-child(2)").text
	disabledOption = context.browser.find_element_by_css_selector("#input-status > option:nth-child(3)").text
	assert('' == defaultOption)
	assert(u'Enabled' == enabledOption)
	assert(u'Disabled' == disabledOption)

@given(u'the phrase "iphone" is entered in product name input')
def step_impl(context):
	context.browser.find_element_by_id("input-name").send_keys("iphone")

@when(u'the user clicks on the "Filter" button')
def step_impl(context):
	context.browser.find_element_by_id("button-filter").click()

@then(u'the following product is shown')
def step_impl(context):
	assert(u'iPhone' == context.browser.find_element_by_css_selector("#form-product tbody > tr:first-child td:nth-child(3)").text)

@given(u'at least one product is shown')
def step_impl(context):
	assert(context.browser.find_element_by_css_selector("#form-product tbody > tr:first-child"))

@when(u'the user clicks on "Edit" button in the "Actions" column')
def step_impl(context):
	context.browser.find_element_by_css_selector("#form-product tbody > tr:first-child td:last-child > a").click()

@then(u'an edit product page is shown')
def step_impl(context):
	assert(u'Edit Product' == context.browser.find_element_by_css_selector("#content h3.panel-title").text)