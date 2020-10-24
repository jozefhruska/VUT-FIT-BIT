import React, { Component } from 'react'
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome'
import './index.scss'
import PageLayout from '../../components/layout'
import HeaderTabs from './header_tabs';

class CoursesDetail extends Component {
	constructor(props) {
		super(props)

		let courseInfo = {}
		switch (this.props.match.params.id) {
			case 'ial':
				courseInfo = {
					type: 'c',
					points: 95,
					crd: true,
					grade: 'A',
				}
				break
			case 'ifj':
				courseInfo = {
					type: 'c',
					points: 45,
					crd: false,
					grade: 'F',
				}
				break
			case 'inm':
				courseInfo = {
					type: 'c',
					points: 80,
					crd: true,
					grade: 'B',
				}
				break
			case 'inp':
				courseInfo = {
					type: 'c',
					points: 85,
					crd: true,
					grade: 'B',
				}
				break
			case 'iss':
				courseInfo = {
					type: 'c',
					points: 30,
					crd: null,
					grade: '4F',
				}
				break
			case 'itu':
				courseInfo = {
					type: 'c',
					points: 100,
					crd: true,
					grade: 'A',
				}
				break
			default:
		}

		this.state = {
			courseId: this.props.match.params.id,
			courseInfo: courseInfo,
		}
	}

	render() {
		return (
			<PageLayout
				title={this.state.courseId.toUpperCase() + ' course detail'}
			>

				<HeaderTabs 
					courseId={this.state.courseId}
					path={this.props.match.path}
				/>

				<div className="course-detail row">
					<div className="col-md-6">
						<div className="course-detail-grades">
							<div className="contentBlock">
								<div className="contentBlock__header">
									Current status
								</div>
								<div className="contentBlock__body">
									<table className="current-status-table hug">
										<tr className="header">
											<td>Abrv</td>
											<td>Type</td>
											<td>Points</td>
											<td>Crd</td>
											<td>Grade</td>
										</tr>
										<tr>
											<td>
												{this.state.courseId.toUpperCase()}
											</td>
											<td>
												{this.state.courseInfo.type}
											</td>
											<td>
												{this.state.courseInfo.points}
											</td>
											<td>
												{this.state.courseInfo.crd ===
												true ? (
													<FontAwesomeIcon icon="check" />
												) : (
													<FontAwesomeIcon icon="times" />
												)}
											</td>
											<td>
												{this.state.courseInfo.grade}
											</td>
										</tr>
									</table>
								</div>
							</div>
						</div>
						<div className="course-detail-practice">
							<div className="contentBlock">
								<div className="contentBlock__header">
									Practice dates
								</div>
								<div className="contentBlock__body">
									<table className="practice-dates-table hug">
										<tbody>
											<tr className="header">
												<td>Practice date</td>
												<td>Count</td>
												<td>Max</td>
												<td>Reg</td>
											</tr>
											<tr>
												<td>
													Monday 10:00 - 12:00 (odd
													weeks)
												</td>
												<td>100</td>
												<td>110</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
											<tr>
												<td>
													Monday 12:00 - 14:00 (odd
													weeks)
												</td>
												<td>12</td>
												<td>110</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
											<tr>
												<td>
													Monday 14:00 - 16:00 (odd
													weeks)
												</td>
												<td>10</td>
												<td>110</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
											<tr>
												<td>
													Tuesday 10:00 - 12:00 (even
													weeks)
												</td>
												<td>90</td>
												<td>110</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
											<tr>
												<td>
													Tuesday 12:00 - 14:00 (even
													weeks)
												</td>
												<td>72</td>
												<td>110</td>
												<td>
													<input
														type="checkbox"
														defaultChecked
													/>
												</td>
											</tr>
											<tr>
												<td>
													Tuesday 14:00 - 16:00 (even
													weeks)
												</td>
												<td>65</td>
												<td>110</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
											<tr>
												<td>
													Tuesday 16:00 - 18:00 (even
													weeks)
												</td>
												<td>71</td>
												<td>110</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
										</tbody>
									</table>
								</div>
							</div>
						</div>
					</div>
					<div className="col-md-6">
						<div className="course-detail-projects">
							<div className="contentBlock">
								<div className="contentBlock__header">
									Projects
								</div>
								<div className="contentBlock__body">
									<table className="projects-table hug">
										<tbody>
											<tr className="header">
												<td>Project topic</td>
												<td>Count</td>
												<td>Max</td>
												<td>Reg</td>
											</tr>
											<tr>
												<td>First project</td>
												<td>100</td>
												<td>110</td>
												<td>
													<input
														type="checkbox"
														defaultChecked
													/>
												</td>
											</tr>
											<tr>
												<td>Second project</td>
												<td>12</td>
												<td>110</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
											<tr>
												<td>Third</td>
												<td>10</td>
												<td>110</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
											<tr>
												<td>Fourth project</td>
												<td>1</td>
												<td>110</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
										</tbody>
									</table>
								</div>
							</div>
						</div>
						<div className="course-detail-examinations">
							<div className="contentBlock">
								<div className="contentBlock__header">
									Examinations
								</div>
								<div className="contentBlock__body">
									<table className="examinations-table hug">
										<tbody>
											<tr className="header">
												<td>Date</td>
												<td>Type</td>
												<td>Count</td>
												<td>Reg</td>
											</tr>
											<tr>
												<td>08. 12. 2018</td>
												<td>The right date</td>
												<td>0</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
											<tr>
												<td>12. 12. 2018</td>
												<td>First correction date</td>
												<td>0</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
											<tr>
												<td>24. 12. 2018</td>
												<td>Second correction date</td>
												<td>0</td>
												<td>
													<input type="checkbox" />
												</td>
											</tr>
										</tbody>
									</table>
								</div>
							</div>
						</div>
					</div>
				</div>
			</PageLayout>
		)
	}
}

export default CoursesDetail
