import React, { Component } from 'react';
import { FontAwesomeIcon } from '@fortawesome/react-fontawesome';
import { Link } from 'react-router-dom';

import PageLayout from '../../components/layout';
import HeaderTabs from '../Courses_detail/header_tabs';

import './index.scss';

class CoursesForum extends Component{
	constructor(props){
		super(props);
		this.state = {
			courseId: this.props.match.params.id
		};
	}

	render(){
		console.log(this.props);
		return(
			<PageLayout
				title={this.state.courseId.toUpperCase() + " forum"}
			>
				<HeaderTabs 
					path={this.props.match.path}
					courseId={this.state.courseId}
				/>
				<div className="course-forum">
					<Link to="/courses/forum-create-thread">
						<button className="btn btn--primary">Create thread</button>
					</Link>

					<div className="course-forum-topics">
						<table>
							<tr className="header">
								<td>Created at</td>
								<td>Topic</td>
								<td>Author</td>
								<td>Replies</td>
								<td>Open</td>
							</tr>

							<tr>
								<td>10. 11. 2018</td>
								<td>What should be the result of 42 in our project?</td>
								<td>Aleš Sedláčik (xsedli00)</td>
								<td>3</td>
								<td><FontAwesomeIcon icon="check" /></td>
							</tr>

							<tr>
								<td>1. 11. 2018</td>
								<td>Column interpretation</td>
								<td>Jano Janovicz (xjanoc00)</td>
								<td>2</td>
								<td><FontAwesomeIcon icon="check" /></td>
							</tr>

							<tr className="closed">
								<td>42. 10. 2018</td>
								<td>Angular forbidden</td>
								<td>Danka Vavrova (xvavro00)</td>
								<td>20</td>
								<td><FontAwesomeIcon icon="times" /></td>
							</tr>
						</table>
					</div>
				</div>
			</PageLayout>
		);
	}
}

export default CoursesForum;