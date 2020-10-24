import React, { Component } from 'react'

function InfoCard(props) {
  return (
		<div className="col-xs-12 col-md-6">
			<div className={"CardWrapper" + (props.last ? ' CardWrapper--last' : '')}>
				<div className={"c-InfoCard" + (props.valid ? ' c-InfoCard--is-valid' : '')}>
					<h3 className="dark hug">{ props.title }</h3>

					<div>
						<p>
							<span>Typ:</span> I<br />
							<span>Platnost od:</span> 2017-07-02<br />
							<span>Platnost do:</span> 2018-07-02
						</p>
					</div>
				</div>
			</div>
		</div>
  );
}

class InfoCards extends Component {
	render() {
		return (
			<div className="InfoCards contentBlock">
				<div className="contentBlock__header">Cards</div>
				<div className="contentBlock__body InfoCards_body">
					<div className="row">
						<InfoCard title="420 333 444 555 M" valid last/>
					</div>
				</div>
			</div>
		)
	}
}

export default InfoCards
