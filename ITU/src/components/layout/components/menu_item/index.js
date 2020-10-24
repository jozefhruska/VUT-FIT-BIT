import React, { Component } from 'react';
import './index.scss';
import '../../../../../node_modules/animate.css/animate.min.css';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';

class MenuItem extends Component {
	constructor(props) {
		super(props)

		this.state = {
			submenuVisible: false,
		}
	}

	render() {
		return (
			<div className="menu-item-wrapper">
				<li
					className={
						this.state.submenuVisible
							? 'menu-item menu-item-active'
							: 'menu-item'
					}
					onClick={() => {
						if (this.props.children) {
							this.setState({
								submenuVisible: !this.state.submenuVisible,
							})
						} else {
							this.props.onRedirect()
						}
					}}
				>

					{this.props.icon ? (
						<FontAwesomeIcon icon={this.props.icon} size="lg" fixedWidth />
					) : null}

					{this.props.label}

					{this.props.parent ? (
						<div className="arrowDown">
							<FontAwesomeIcon icon="sort-down" fixedWidth/>
						</div>
					) : null}
				</li>

				{this.props.children && this.state.submenuVisible ? (
					<li className="menu-item-submenu">{this.props.children}</li>
				) : null}
			</div>
		)
	}
}

export default MenuItem;
