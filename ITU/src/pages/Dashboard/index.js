import React, { Component } from 'react'

import './index.scss'
import InfoIcon from '../../assets/icons/info.png'

import PageLayout from '../../components/layout'
import Alert from '../../components/alert'
import Timetable from './components/personal_timetable'
import CurrentDates from './components/current_dates'
import Grades from './components/grades'

class Dashboard extends Component {
	render() {
		return (
			<PageLayout title="Dashboard" ntp>
				<Alert
					icon={InfoIcon}
					label="ITU - Project registration starts tonight at 8pm"
				/>

				<Timetable />

				<div className="row">
					<div className="col-xs-12 col-md-6">
						<CurrentDates />
					</div>
					<div className="col-xs-12 col-md-6">
						<Grades />
					</div>
				</div>
			</PageLayout>
		)
	}
}

export default Dashboard
