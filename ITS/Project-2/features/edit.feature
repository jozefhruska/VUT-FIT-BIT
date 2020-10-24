Feature: Product edit

	Background:
		Given a web browser is on a product edit page

	Scenario: Product name change
		Given the "General" tab is selected in tab menu
		And the user adds "42" to the "Product name" input
		When the user clicks on the "Save" button
		Then product edit page is reloaded
		And the "Product name" value is "iMac42"

	Scenario Outline: Label tooltip show
		Given the "Data" tab is selected in tab menu
		When the user hovers on <label> tooltip icon
		Then the <tooltip> is shown

		Examples:
		  | label | tooltip                            |
			| SKU   | Stock Keeping Unit                 |
			| UPC   | Universal Product Code             |
			| EAN   | European Article Number            |
			| JAN   | Japanese Article Number            |
			| ISBN  | International Standard Book Number |
			| MPN   | Manufacturer Part Number           |

	Scenario: Calendar selector show
		Given the "Data" tab is selected in tab menu
		When the user clicks on calendar icon for "Date Available" input
		Then a calendar selector is shown below the input

	Scenario: Wrong model value
		Given the "Data" tab is selected in tab menu
		And an empty value is in "Model" input
		When the user clicks on the "Save" button
		Then a warning is displayed below the page title
		And an explanation is displayed below the "Model" input

	Scenario: Remove product from a category
		Given the "Links" tab is selected in tab menu
		And products is in at least one category
		When the user clicks on the remove icon next to a category name
		Then category dissapears from the list
		And the product is no longer in this category

	Scenario: Remove a product image
		Given the "Image" tab is selected in tab menu
		And products has at least one image
		When the user clicks on the remove icon in the last table column
		Then image dissapears from the list