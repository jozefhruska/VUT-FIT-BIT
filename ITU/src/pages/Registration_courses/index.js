import React, { Component } from 'react'

import './index.scss'
import RegRules from '../../assets/registration_rules.pdf'

import PageLayout from '../../components/layout'
import SemesterCourses from './components/semester_courses'

class RegistrationCourses extends Component {
	render() {
		return (
			<PageLayout title="Courses registration">
				<div className="contentBlock contentBlock--ntm">
					<div className="contentBlock__body">
						<p className="hug">
							Before your registration, please read:{' '}
							<a
								href={RegRules}
								target="_blank"
								rel="noopener noreferrer"
							>
								registration rules
							</a>
							.
						</p>

						<p>
							<b>Year credit limit: 70 cr.</b> (decisive period
							passed: 36 cr./sem, WA: 2,458, failed: 0, failed
							C/CE: 0)
							<br />
							<b>Passed: 61 cr., C+CE 56 cr., E 5 cr.</b>{' '}
							(available 27+4)
							<br />
							<b>
								Register at least IBT+ISZ in the last year,
								otherwise at least 30 cr., max. 70 cr.
							</b>
						</p>

						<div className="table_responsive">
							<table className="registered-credits">
								<tbody>
									<tr className="header">
										<td>C</td>
										<td>CE</td>
										<td>E</td>
										<td>R</td>
										<td>O</td>
										<td>Total</td>
									</tr>
									<tr>
										<td>50</td>
										<td>4</td>
										<td>9</td>
										<td>0</td>
										<td>0</td>
										<td className="highlight">
											63 credits
										</td>
									</tr>
								</tbody>
							</table>
						</div>
					</div>
				</div>

				<div className="contentBlock">
					<div className="contentBlock__header">Winter semester</div>
					<div className="contentBlock__body">
						<SemesterCourses />
					</div>
				</div>

				<div className="contentBlock">
					<div className="contentBlock__header">Summer semester</div>
					<div className="contentBlock__body">
						<SemesterCourses />
					</div>
				</div>
			</PageLayout>
		)
	}
}

export default RegistrationCourses
