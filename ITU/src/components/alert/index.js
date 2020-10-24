import React, { Component } from 'react'

import './index.scss'
import CrossIcon from '../../assets/icons/cross.png'

class Alert extends Component {
	constructor(props) {
		super(props)

		this.state = {
			visible: true,
		}
	}

	render() {
		return (
			<div>
				{this.state.visible ? (
					<div className="alert animated shake">
						<img src={this.props.icon} className="info-icon" alt="info icon" />
						<p>{this.props.label}</p>
						<img
							src={CrossIcon}
							className="cross-icon"
							onClick={() => {
								this.setState({ visible: false })
							}}
							alt="close icon"
						/>
					</div>
				) : null}
			</div>
		)
	}
}

export default Alert
