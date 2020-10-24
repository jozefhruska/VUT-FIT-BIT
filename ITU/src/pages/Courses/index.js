import React, { Component } from 'react';
import './index.scss';

import PageLayout from '../../components/layout';
import SemesterCourses from './components/semester_courses';

class RegistrationBachelorThesis extends Component{
	render(){
		return(
			<PageLayout
				title="Enrolled courses"
			>
				<div className="courses-info contentBlock contentBlock--ntm">
					<div className="contentBlock__header">Winter semester</div>
					<div className="winter-semester contentBlock__body">
						<SemesterCourses/>
					</div>
				</div>

				<div className="courses-info contentBlock">
				<div className="contentBlock__header">Summer semester</div>
					<div className="summer-semester contentBlock__body">
						<SemesterCourses/>
					</div>
				</div>
			</PageLayout>
		);
	}
}

export default RegistrationBachelorThesis;