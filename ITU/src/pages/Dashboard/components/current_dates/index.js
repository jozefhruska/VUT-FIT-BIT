import React, { Component } from 'react'

import './index.scss'
import CurrentDateItem from '../current_date_item'

class CurrentDates extends Component {
	render() {
		return (
			<div className="current-dates contentBlock">
				<div className="contentBlock__header">Current dates</div>

				<div className="current-dates-content contentBlock__body">
					<CurrentDateItem
						date="30. 11. 2018"
						label="Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Phasellus enim erat, vestibulum vel, aliquam a, posuere eu, velit."
					/>
					<CurrentDateItem
						date="20. 12. 2018"
						label="Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Phasellus enim erat, vestibulum vel, aliquam a, posuere eu, velit."
					/>
					<CurrentDateItem
						date="23. 12. 2018"
						label="Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Phasellus enim erat, vestibulum vel, aliquam a, posuere eu, velit."
					/>
					<CurrentDateItem
						date="31. 12. 2018"
						label="Lorem ipsum dolor sit amet, consectetuer adipiscing elit. Phasellus enim erat, vestibulum vel, aliquam a, posuere eu, velit."
					/>
				</div>
			</div>
		)
	}
}

export default CurrentDates
