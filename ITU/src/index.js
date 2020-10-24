import React from 'react'
import ReactDOM from 'react-dom'
import { BrowserRouter as Router, Route } from 'react-router-dom'
import { library } from '@fortawesome/fontawesome-svg-core'
import { faSignOutAlt, faBars, faFile, faAt, faExclamationTriangle, faCheckDouble, faTh, faBook, faSignInAlt, faUserGraduate, faMemory, faSortDown, faCheck, faTimes } from '@fortawesome/free-solid-svg-icons'

import Dashboard from './pages/Dashboard';
import Courses from './pages/Courses';
import CoursesDetail from './pages/Courses_detail';
import CoursesForum from './pages/Courses_forum';
import ForumCreateThread from './pages/Create_forum_thread';
import Login from './pages/Login';
import RegistrationCourses from './pages/Registration_courses';
import RegistrationBachelorThesis from './pages/Registration_bachelor_thesis';
import RegistrationProjectPractice from './pages/Registration_project_practice';
import AccountInfo from './pages/Account/pages/Info';
import AccountsCards from './pages/Account/pages/AccountsCards';
import AccountCurrentStudy from './pages/Account/pages/CurrentStudy';
import OthersFTP from './pages/Others_ftp';
import OthersAdministrativeProcedures from './pages/Others_administrative_procedures';
import OthersAdministrativeProceduresCreate from './pages/Others_administrative_procedures_create';
import TimetableEdit from './pages/Timetable_edit';

import './styles/style.scss'

library.add([
	faSignOutAlt, faBars, faFile, faAt, faExclamationTriangle, faCheckDouble, faBook, faTh, faSortDown, faSignInAlt, faUserGraduate, faMemory, faCheck, faTimes
]);

const AppRouter = () => (
	<Router>
		<div>
			<Route path="/" exact component={Login} />
			<Route path="/dashboard" exact component={Dashboard} />
			<Route path="/courses" exact component={Courses} />
			<Route path="/courses/detail/:id" exact render={(props) => <CoursesDetail {...props} />}  />
			<Route path="/courses/forum/:id" exact render={(props) => <CoursesForum {...props} />}  />
			<Route path="/courses/forum-create-thread" exact component={ForumCreateThread} />
			<Route path="/registration/courses" exact component={RegistrationCourses} />
			<Route path="/registration/bachelor_thesis" exact component={RegistrationBachelorThesis} />
			<Route path="/registration/project_practice" exact component={RegistrationProjectPractice} />
			<Route path="/account/info" exact component={AccountInfo} />
			<Route path="/account/accounts_cards" exact component={AccountsCards} />
			<Route path="/account/current_study" exact component={AccountCurrentStudy} />
			<Route path="/others/ftp" exact component={OthersFTP} />
			<Route path="/others/administrative_procedures" exact component={OthersAdministrativeProcedures} />
			<Route path="/others/administrative_procedures/create" exact component={OthersAdministrativeProceduresCreate} />
			<Route path="/timetable/edit" exact component={TimetableEdit} />
		</div>
	</Router>
);

ReactDOM.render(<AppRouter />, document.getElementById('root'));
