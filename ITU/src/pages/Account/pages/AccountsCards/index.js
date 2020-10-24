import React, { Component } from 'react'

import './index.scss'

import PageLayout from '../../../../components/layout'
import Accounts from './components/Accounts/Accounts'
import Cards from './components/Cards/Cards'
import AccountTabs from '../../components/AccountTabs'

class AccountsCards extends Component {
	render() {
		return (
			<PageLayout title="Account">
				<AccountTabs location={this.props.location} />

				<div className="row">
					<div className="col-xs-12 col-md-6">
						<Accounts />
					</div>

					<div className="col-xs-12 col-md-6">
						<Cards />
					</div>
				</div>
			</PageLayout>
		)
	}
}

export default AccountsCards
