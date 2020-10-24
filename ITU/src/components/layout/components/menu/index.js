import React, { Component } from 'react'
import { withRouter } from 'react-router-dom'

import './index.scss'
import WithdrawalStudent from '../../../../assets/withdrawal_student.pdf'
import StudyConfirmation from '../../../../assets/potvrzostud_en.pdf'

import MenuItem from '../menu_item'

class Menu extends Component {
	constructor(props) {
		super(props)

		this.openInNewTab = this.openInNewTab.bind(this)
	}

	render() {
		return (
			<div className="menu-wrapper">
				<ul>
					<MenuItem
						icon="th"
						label="Dashboard"
						onRedirect={() => {
							this.props.history.push('/dashboard')
						}}
					/>

					<MenuItem
						icon="book"
						label="Courses"
						onRedirect={() => {
							this.props.history.push('/courses')
						}}
					/>

					<MenuItem icon="sign-in-alt" label="Registration" parent>
						<ul>
							<MenuItem
								label="Courses"
								onRedirect={() => {
									this.props.history.push(
										'/registration/courses'
									)
								}}
							/>
							<MenuItem
								label="Bachelor's thesis"
								onRedirect={() => {
									this.props.history.push(
										'/registration/bachelor_thesis'
									)
								}}
							/>
							<MenuItem
								label="Project practice"
								onRedirect={() => {
									this.props.history.push(
										'/registration/project_practice'
									)
								}}
							/>
						</ul>
					</MenuItem>

					<MenuItem
						icon="user-graduate"
						label="My account"
						onRedirect={() => {
							this.props.history.push('/account/info')
						}}
					/>

					<MenuItem icon="memory" label="Others" parent>
						<ul>
							<MenuItem
								label="Administrative procedures"
								onRedirect={() => {
									this.props.history.push('/others/administrative_procedures')
								}}
							/>
							<MenuItem
								label="Assessment of teaching"
								onRedirect={() => {
									this.props.history.push('/')
								}}
							/>
							<MenuItem
								label="Room reservation"
								onRedirect={() => {
									this.props.history.push('/')
								}}
							/>
							<MenuItem
								label="Event registration"
								onRedirect={() => {
									this.props.history.push('/')
								}}
							/>
							<MenuItem
								label="Web mail"
								onRedirect={() => {
									window.location =
										'https://email.fit.vutbr.cz/'
								}}
							/>
							<MenuItem
								label="Confirmation of the study"
								onRedirect={() => {
									this.openInNewTab(StudyConfirmation)
								}}
							/>
							<MenuItem
								label="Notification of Withdrawal from Studies"
								onRedirect={() => {
									this.openInNewTab(WithdrawalStudent)
								}}
							/>
						</ul>
					</MenuItem>
				</ul>
			</div>
		)
	}

	openInNewTab(url) {
		let win = window.open(url, '_blank')
		win.focus()
	}
}

export default withRouter(Menu)
