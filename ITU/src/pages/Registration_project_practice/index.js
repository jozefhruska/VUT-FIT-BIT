import React, { Component } from 'react'

import PageLayout from '../../components/layout'
import InfoIcon from '../../assets/icons/info.png'

import Alert from '../../components/alert'

class RegistrationProjectPractice extends Component {
	render() {
		return (
			<PageLayout title="Project practice registration" ntp>
				<Alert
					icon={InfoIcon}
					label="You do not have any project practice registered."
				/>
			</PageLayout>
		)
	}
}

export default RegistrationProjectPractice
