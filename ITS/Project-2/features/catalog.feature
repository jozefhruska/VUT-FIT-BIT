Feature: Product Catalog

	Background:
		Given an e-shop is selling the iPhone product
		And the user is signed in

	Scenario: Product name autocomplete in filters
		Given a web browser is on the product catalog page
		And the product name input is focused
		When the phrase "ipho" is entered
		Then the dropdown menu is visible:
			| iPhone |

	Scenario: Filter by status dropdown options
		Given a web browser is on the product catalog page
		When the user clicks on the status input
		Then status options in dropdown are shown:
			| ✓           |
			|    Enabled  |
			|    Disabled |

	Scenario: Filter products by a product name
		Given a web browser is on the product catalog page
		And the phrase "iphone" is entered in product name input
		When the user clicks on the "Filter" button
		Then the following product is shown:
			| iPhone |

	Scenario: Open a product edit page
		Given a web browser is on the product catalog page
		And at least one product is shown
		When the user clicks on "Edit" button in the "Actions" column
		Then an edit product page is shown