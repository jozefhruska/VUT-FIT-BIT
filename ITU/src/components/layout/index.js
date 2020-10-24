import React, { Component } from 'react'
import { Link } from 'react-router-dom'
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'

import './index.scss'
import FITLogo from '../../assets/vut_fit_logo_small.png'
import Menu from './components/menu'

class Layout extends Component {
	constructor(props) {
		super(props)

		this.state = {
			menuVisible: false,
		}
	}

	render() {
		return (
			<div className="page-layout">
				<div className="c-siteHeader">
					<div className="c-siteHeader__section">
						<button
							className="c-siteHeader__toggle"
							onClick={() => {
								this.setState({
									menuVisible: !this.state.menuVisible,
								})
							}}
						>
							<FontAwesomeIcon icon="bars" />
						</button>

						<div className="c-siteHeader__brand">
							<Link to="/dashboard">
								<img src={FITLogo} alt="BUT FIT" />
							</Link>
						</div>

						<div className="c-siteHeader__title">
							{this.props.title}
						</div>
					</div>

					<div className="c-siteHeader__section">
						<div className="c-siteHeader__user">
							<span className="name">Jan Demel (xdemel01)</span>
							<Link to="/">
								<FontAwesomeIcon icon="sign-out-alt" />
							</Link>
						</div>
					</div>
				</div>

				<div className="page-layout-content">
					<div className="menuDummy" />

					<div
						className={
							'menu' +
							(this.state.menuVisible ? ' menu--is-visible' : '')
						}
					>
						<Menu />
					</div>

					<div className={"content" + (this.props.ntp ? ' ntp' : '')}>{this.props.children}</div>
				</div>
			</div>
		)
	}
}

export default Layout
