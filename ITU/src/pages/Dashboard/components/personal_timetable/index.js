import React, { Component } from 'react'
import { Link } from 'react-router-dom'

import './index.scss'

class Timetable extends Component {
	render() {
		return (
			<div className="personal-timetable contentBlock">
				<div className="contentBlock__header">Personal timetable</div>

				<div className="timetable table_responsive contentBlock__body">
					<table>
						<tbody>
							<tr>
								<td className="filled_bg_header" />
								<td className="filled_bg_header">8:00</td>
								<td className="filled_bg_header">9:00</td>
								<td className="filled_bg_header">10:00</td>
								<td className="filled_bg_header">11:00</td>
								<td className="filled_bg_header">12:00</td>
								<td className="filled_bg_header">13:00</td>
								<td className="filled_bg_header">14:00</td>
								<td className="filled_bg_header">15:00</td>
								<td className="filled_bg_header">16:00</td>
								<td className="filled_bg_header">17:00</td>
							</tr>

							<tr>
								<td className="filled_bg_header">Mon</td>
								<td className="filled_bg_practice" colSpan="2">
									ITU
								</td>
								<td />
								<td className="filled_bg_lecture" colSpan="2">
									INM
								</td>
								<td colSpan="5" />
							</tr>

							<tr>
								<td className="filled_bg_header">Tue</td>
								<td className="filled_bg_lecture" colSpan="3">
									ISS
								</td>
								<td className="filled_bg_lecture" colSpan="3">
									IAL
								</td>
								<td />
								<td className="filled_bg_practice" colSpan="2">
									ISS
								</td>
								<td />
							</tr>

							<tr>
								<td className="filled_bg_header">Wed</td>
								<td className="filled_bg_lecture" colSpan="3">
									INP
								</td>
								<td />
								<td className="filled_bg_lecture" colSpan="2">
									ITU
								</td>
								<td />
								<td className="filled_bg_sport">SD</td>
								<td colSpan="2" />
							</tr>
							<tr>
								<td className="filled_bg_header">Thu</td>
								<td className="filled_bg_lecture" colSpan="3">
									IFJ
								</td>
								<td className="filled_bg_practice_lecture">
									INP
								</td>
								<td />
								<td className="filled_bg_practice" colSpan="2">
									INM
								</td>
								<td colSpan="3" />
							</tr>
						</tbody>
					</table>

					<div className="edit-timetable-button">
						<Link to="/timetable/edit">
							<button className="btn btn--primary">
								Edit timetable
							</button>
						</Link>
					</div>
				</div>
			</div>
		)
	}
}

export default Timetable
