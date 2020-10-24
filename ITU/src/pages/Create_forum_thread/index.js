import React, { Component } from 'react';

import PageLayout from '../../components/layout';

import './index.scss';

class CreateThread extends Component{
	render(){
		return(
			<PageLayout
				title="Create new thread"
			>
				<div className="create-new-thread contentBlock">
					<div className="contentBlock__body">
						<form>
							<input type="text" placeholder="Title"/>
							<textarea rows="10" placeholder="Message"/>
							<button type="send" className="btn btn--primary">Send</button>
						</form>
					</div>
				</div>
			</PageLayout>
		);
	}
}

export default CreateThread;