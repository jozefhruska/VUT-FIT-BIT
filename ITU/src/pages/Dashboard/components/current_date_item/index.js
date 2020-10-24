import React from 'react'

import './index.scss'

const CurrentDateItem = props => {
	return (
		<div className="current-date-item">
			<div className="current-date-item-date">{props.date}</div>
			<div className="current-date-item-label">{props.label}</div>
		</div>
	)
}

export default CurrentDateItem
