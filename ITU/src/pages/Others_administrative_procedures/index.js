import React, { Component } from 'react'
import { Link } from 'react-router-dom'

import './index.scss'
import PageLayout from '../../components/layout'

class OthersAdministrativeProcedures extends Component {
	render() {
		return (
			<PageLayout title="Administrative procedures">
				<div className="administrative-procedures">
					<div className="new-application-button">
						<Link to="/others/administrative_procedures/create">
							<button className="btn btn--primary">
								New application
							</button>
						</Link>
					</div>

					<div className="table-of-procedures contentBlock">
						<div className="contentBlock__body">
							<table class="hug">
								<tbody>
									<tr className="header">
										<td>Study</td>
										<td>Begin</td>
										<td>Decision</td>
										<td>Category</td>
										<td>Type</td>
										<td>Status</td>
									</tr>

									<tr>
										<td>Number 1</td>
										<td>2018-01-05</td>
										<td>2018-01-11</td>
										<td>Curriculum</td>
										<td>
											Change of the leve of English by 1
											level
										</td>
										<td>Closed</td>
									</tr>

									<tr>
										<td>Number 2</td>
										<td>2019-01-05</td>
										<td>2019-01-11</td>
										<td>Curriculum</td>
										<td>
											Change of the leve of English by 2
											levels
										</td>
										<td>Closed</td>
									</tr>
								</tbody>
							</table>
						</div>
					</div>
				</div>
			</PageLayout>
		)
	}
}

export default OthersAdministrativeProcedures
