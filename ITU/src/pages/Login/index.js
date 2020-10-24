import React, { Component } from 'react'

import './styles.scss'
import FITLogo from '../../assets/vut_fit_logo.png'
import LoginForm from './components/LoginForm'

class Login extends Component {
	render() {
		return (
			<div className="login-page">
				<div className="left-column">
					<div className="school-logo">
						<img
							src={FITLogo}
							className="login-page-fit-logo"
							alt="BUT FIT"
						/>
					</div>
				</div>

				<div className="right-column">
					<LoginForm
						onSubmitClick={() => {
							this.props.history.push('/dashboard')
						}}
					/>
				</div>
			</div>
		)
	}
}

export default Login
