import React from 'react'
import { Link } from 'react-router-dom'

import './index.scss';

function AccountTabs(props) {
  return (
		<div className="c-AccountTabs">
			<Link to="/account/info" className={"c-AccountTabs__item" + (props.location.pathname === '/account/info' ? ' c-AccountTabs__item--is-active' : '')}>
				Personal information
			</Link>

			<Link to="/account/accounts_cards" className={"c-AccountTabs__item" + (props.location.pathname === '/account/accounts_cards' ? ' c-AccountTabs__item--is-active' : '')}>
				Accounts & cards
			</Link>

			<Link to="/account/current_study" className={"c-AccountTabs__item" + (props.location.pathname === '/account/current_study' ? ' c-AccountTabs__item--is-active' : '')}>
				Transcript of current study
			</Link>
		</div>
  );
}

export default AccountTabs;
