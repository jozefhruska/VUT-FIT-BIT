import React, { Component } from 'react'

import './index.scss'

import PageLayout from '../../../../components/layout'
import AccountTabs from '../../components/AccountTabs'

import ProfilePhoto from '../../../../assets/profile_photo.jpg'

class AccountInfo extends Component {
	render() {
		return (
			<PageLayout title="Account">
				<AccountTabs location={this.props.location} />

				<div className="row">
					<div className="col-xs-12 col-md-4">
						<div className="AccountInfoBasic contentBlock">
							<div className="contentBlock__body contentBlock__body--flex-center">
								<div className="AccountInfoBasic__photo">
									<img src={ProfilePhoto} alt="user" />
								</div>

								<div className="AccountInfoBasic__name">
									Jan Demel
								</div>
								<div className="AccountInfoBasic__meta">
									2nd year BIT, full-time, FIT
								</div>
								<div className="AccountInfoBasic__validity">
									Account valid
								</div>
							</div>
						</div>
					</div>

					<div className="col-xs-12 col-md-8">
						<div className="contentBlock">
							<div className="contentBlock__header">
								Account details
							</div>
							<div className="contentBlock__body contentBlock__body--table">
								<div className="row">
									<div className="col-4">
										<p className="hug">
											<span>First name:</span>
											<br />
											Jan
										</p>
									</div>

									<div className="col-4">
										<p className="hug">
											<span>Family name:</span>
											<br />
											Demel
										</p>
									</div>

									<div className="col-4">
										<p className="hug">
											<span>Sex:</span>
											<br />
											Male
										</p>
									</div>
								</div>

								<div className="row">
									<div className="col-4">
										<p className="hug">
											<span>Qualification:</span>
											<br />
										</p>
									</div>

									<div className="col-4">
										<p className="hug">
											<span>Nationality:</span>
											<br />
											Slovak
										</p>
									</div>

									<div className="col-4">
										<p className="hug">
											<span>Birth number:</span>
											<br />
											9710103222
										</p>
									</div>
								</div>

								<div className="row">
									<div className="col-4">
										<p className="hug">
											<span>Email:</span>
											<br />
											xdemel01@stud.fit.vutbr.cz
										</p>
									</div>

									<div className="col-4">
										<p className="hug">
											<span>Birth date:</span>
											<br />
											10.10. 1997
										</p>
									</div>
								</div>
							</div>
						</div>

						<div className="row">
							<div className="col-xs-12 col-md-6">
								<div className="contentBlock">
									<div className="contentBlock__header">
										Permanent address
									</div>

									<div className="contentBlock__body contentBlock__body--table">
										<div className="row">
											<div className="col-6">
												<p className="hug">
													<span>Street</span>
													<br />
													Palackého 2693/13
												</p>
											</div>

											<div className="col-6">
												<p className="hug">
													<span>City</span>
													<br />
													Trenčín
												</p>
											</div>
										</div>

										<div className="row">
											<div className="col-6">
												<p className="hug">
													<span>Postal code</span>
													<br />
													911 01
												</p>
											</div>

											<div className="col-6">
												<p className="hug">
													<span>Country</span>
													<br />
													Slovakia
												</p>
											</div>
										</div>

										<div className="row">
											<div className="col-6">
												<p className="hug">
													<span>Phone</span>
													<br />
													+421 999 888 222
												</p>
											</div>

											<div className="col-6">
												<p className="hug">
													<span>Email</span>
													<br />
													email@vutbr.cz
												</p>
											</div>
										</div>
									</div>
								</div>
							</div>

							<div className="col-xs-12 col-md-6">
								<div className="contentBlock">
									<div className="contentBlock__header">
										Contact address for letter mail
									</div>

									<div className="contentBlock__body contentBlock__body--table">
										<div className="row">
											<div className="col-6">
												<p className="hug">
													<span>Street</span>
													<br />
													Palackého 2693/13
												</p>
											</div>

											<div className="col-6">
												<p className="hug">
													<span>City</span>
													<br />
													Trenčín
												</p>
											</div>
										</div>

										<div className="row">
											<div className="col-6">
												<p className="hug">
													<span>Postal code</span>
													<br />
													911 01
												</p>
											</div>

											<div className="col-6">
												<p className="hug">
													<span>Country</span>
													<br />
													Slovakia
												</p>
											</div>
										</div>

										<div className="row">
											<div className="col-6">
												<p className="hug">
													<span>Phone</span>
													<br />
													+421 999 888 222
												</p>
											</div>

											<div className="col-6">
												<p className="hug">
													<span>Email</span>
													<br />
													email@vutbr.cz
												</p>
											</div>
										</div>
									</div>
								</div>
							</div>
						</div>
					</div>
				</div>
			</PageLayout>
		)
	}
}

export default AccountInfo
