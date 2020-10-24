import React, { Component } from 'react'
import './index.scss'

import PageLayout from '../../components/layout'

class RegistrationBachelorThesis extends Component {
	render() {
		return (
			<PageLayout title="Bachelor's thesis registration">
				<div className="contentBlock contentBlock--ntm">
					<div className="contentBlock__body">
						<div className="thesis-table">
							<table className="hug">
								<tbody>
									<tr className="header">
										<td>Abbrv</td>
										<td>List of topics</td>
										<td>Enrolled</td>
										<td>Registration status</td>
									</tr>
									<tr>
										<td>IBP</td>
										<td>Bachelor's Thesis</td>
										<td />
										<td>registration finished</td>
									</tr>
								</tbody>
							</table>
						</div>

						<p>
							Before you submit your thesis you should read these:
							<br />
						</p>

						<ul>
							<li>
								<a
									href="http://www.fit.vutbr.cz/info/szz/harmonogram.php.en"
									target="_blank"
									rel="noopener noreferrer"
								>
									Schedule for Drawing up Bachelor's and
									Master's Theses
								</a>
							</li>
							<li>
								<a
									href="http://www.fit.vutbr.cz/info/szz/.en"
									target="_blank"
									rel="noopener noreferrer"
								>
									Final State Examination
								</a>
							</li>
						</ul>
					</div>
				</div>
			</PageLayout>
		)
	}
}

export default RegistrationBachelorThesis
