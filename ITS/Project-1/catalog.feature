Feature: Product Catalog

	Background:
		Given an e-shop is selling the iPhone product

	Scenario: Product name autocomplete in filters
		Given a web browser is on the product catalog page
		And the product name input is focused
		When the phrase "ipho" is entered
		Then the dropdown menu is visible:
			| iPhone |

	Scenario: Filter by status dropdown options
		Given a web browser is on the product catalog page
		When the user clicks on the status input
		Then an status options in dropdown are shown:
			| ✓           |
			|    Enabled  |
			|    Disabled |

	Scenario: Filter products by a product name
		Given a web browser is on the product catalog page
		When the phrase "iphone" is entered
		Then the results for "iphone" are shown
		And the following product is shown:
			| iPhone |

	Scenario: Open a product edit page
		Given a web browser is on the product catalog page
		And at least one product is shown
		When the user clicks on "Edit" button in the "Actions" column
		Then an edit product page is shown