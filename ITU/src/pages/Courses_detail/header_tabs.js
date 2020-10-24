import React from 'react';

import { Link } from 'react-router-dom';

const HeaderTabs = (props) => {
	return(
		<div className="c-CoursesTabs">
			<Link to={"/courses/detail/" + props.courseId} className={"c-CoursesTabs__item" + (props.path == '/courses/detail/:id' ? ' c-CoursesTabs__item--is-active' : '')}>
				Course detail
			</Link>

			<Link to={"/courses/forum/" + props.courseId} className={"c-CoursesTabs__item" + (props.path == '/courses/forum/:id' ? ' c-CoursesTabs__item--is-active' : '')}>
				Forum
			</Link>
		</div>
	);
}

export default HeaderTabs;