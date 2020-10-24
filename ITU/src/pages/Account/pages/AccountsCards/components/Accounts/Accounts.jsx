import React, { Component } from 'react'

function InfoAccount(props) {
  return (
		<div className="col-xs-12 col-md-6">
			<div className={"AccountWrapper" + (props.last ? ' AccountWrapper--last' : '')}>
				<div className="c-InfoAccount">
					<h3 className="dark hug">{ props.title }</h3>
					<p>{ props.subtitle }</p>
				</div>
			</div>
		</div>
  );
}

class InfoAccounts extends Component {
	render() {
		return (
			<div className="InfoAccounts contentBlock">
				<div className="contentBlock__header">Accounts</div>
				<div className="contentBlock__body InfoAccounts_body">
					<div className="row">
						<InfoAccount title="Change FIT password" subtitle="Change password for Unix, wis, cas."/>
						<InfoAccount title="Microsoft Active Directory password" subtitle="For Windows and file server fik."/>
						<InfoAccount title="FIT Radius server account" subtitle="For wired and WiFi connection to network."/>
						<InfoAccount title="MySQL database account on eva" subtitle="For student projects."/>
						<InfoAccount title="BUT account password" subtitle="For BUT IS access, WiFi (SSID=VUTBRNO) requires VUTpin." last/>
					</div>
				</div>
			</div>
		)
	}
}

export default InfoAccounts
