import React, { Component } from 'react'

import './index.scss'
import Checked from '../../../../assets/icons/checked.png'
import Cross from '../../../../assets/icons/cross.png'
import { Link, withRouter } from 'react-router-dom'

class Grades extends Component {
	render() {
		return (
			<div className="grades contentBlock">
				<div className="contentBlock__header">Grades</div>

				<div className="grades-content table_responsive contentBlock__body">
					<table>
						<tbody>
							<tr className="grades-header">
								<td>Abrv</td>
								<td>Type</td>
								<td>Points</td>
								<td>Crd</td>
								<td>Grade</td>
							</tr>
							<tr
								className="grades-passed"
								onClick={() => {
									this.props.history.push(
										'/courses/detail/ial'
									)
								}}
							>
								<td>IAL</td>
								<td>C</td>
								<td>95</td>
								<td>
									<img src={Checked} alt="check" />
								</td>
								<td>A</td>
							</tr>
							<tr
								className="grades-failed"
								onClick={() => {
									this.props.history.push(
										'/courses/detail/ifj'
									)
								}}
							>
								<td>IFJ</td>
								<td>C</td>
								<td>45</td>
								<td>
									<img src={Cross} alt="check" />
								</td>
								<td>F</td>
							</tr>
							<tr
								className="grades-passed"
								onClick={() => {
									this.props.history.push(
										'/courses/detail/inm'
									)
								}}
							>
								<td>INM</td>
								<td>C</td>
								<td>80</td>
								<td>
									<img src={Checked} alt="check" />
								</td>
								<td>B</td>
							</tr>
							<tr
								className="grades-passed"
								onClick={() => {
									this.props.history.push(
										'/courses/detail/inp'
									)
								}}
							>
								<td>INP</td>
								<td>C</td>
								<td>85</td>
								<td>
									<img src={Checked} alt="check" />
								</td>
								<td>B</td>
							</tr>
							<tr
								onClick={() => {
									this.props.history.push(
										'/courses/detail/iss'
									)
								}}
							>
								<td>ISS</td>
								<td>C</td>
								<td>30</td>
								<td />
								<td>4F</td>
							</tr>
							<tr
								className="grades-passed"
								onClick={() => {
									this.props.history.push(
										'/courses/detail/itu'
									)
								}}
							>
								<td>ITU</td>
								<td>C</td>
								<td>100</td>
								<td>
									<img src={Checked} alt="check" />
								</td>
								<td>A</td>
							</tr>
						</tbody>
					</table>

					<div className="show-more-grades">
						<Link to="/courses">
							<button className="btn btn--primary">
								Show more
							</button>
						</Link>
					</div>
				</div>
			</div>
		)
	}
}

export default withRouter(Grades)
