import React, { Component } from 'react'

import './index.scss'
import PageLayout from '../../components/layout'

class OthersAdministrativeProceduresCreate extends Component {
	render() {
		return (
			<PageLayout title="Create administrative procedure">
				<div className="administrative-procedures-create contentBlock contentBlock--ntm">
					<form className="contentBlock__body">
						<select defaultValue="0">
							<option value="0">
								Select
							</option>
							<option value="19">
								course - Individual study plan in the course
							</option>
							<option value="13">
								course - Makeup exam date
							</option>
							<option value="1">
								course - Review of exam results in the course
							</option>
							<option value="2">
								course - Review of the credit result from the
								course
							</option>
							<option value="20">
								curriculum - Individual study plan for enrolment
								of a repeated elective course in SS of current
								academic year
							</option>
							<option value="10">
								curriculum - Postponement of duty to repeat
								uncompleted obligatory courses
							</option>
							<option value="17">
								curriculum - Exception for 3rd enrollment of the
								course
							</option>
							<option value="21">
								curriculum - Enrolment or cancelling of the
								enrolment of the course
							</option>
							<option value="5">
								curriculum - Enrolment of an external language
								course and promise of reimbursement of the
								teaching
							</option>
							<option value="24">
								curriculum - Change of the level of English by 1
								level
							</option>
							<option value="42">
								curriculum - Change of the level of English by 2
								or more levels
							</option>
							<option value="12">
								curriculum - Cancellation of the thesis and the
								Final State Examination
							</option>
							<option value="22">
								curriculum - Cancellation of course enrolment
							</option>
							<option value="58">
								curriculum - Cancelling the courses during
								Erasmus trip
							</option>
							<option value="71">
								exit - Electronic confirmation of the graduate
								exit sheet
							</option>
							<option value="70">
								exit - Electronic confirmation of the student
								exit sheet
							</option>
							<option value="74">
								notification - Update of personal information
							</option>
							<option value="66">
								notification - Recording an impairment during
								studies
							</option>
							<option value="6">
								recognition - Recognition of the language course
								from FME/FCE and payment of tuition
							</option>
							<option value="63">
								recognition - Recognition of the course BAN4
							</option>
							<option value="67">
								recognition - Recognition of the course from the
								previous study at another faculty as a FIT
								course
							</option>
							<option value="30">
								recognition - Recognition of the course from the
								study abroad as a FIT course
							</option>
							<option value="28">
								recognition - Recognition of the courses from
								previous studies at FIT
							</option>
							<option value="29">
								recognition - Recognition of the courses from
								the previous study at another faculty
							</option>
							<option value="25">
								recognition - Recognition of the courses from
								parallel studies
							</option>
							<option value="31">
								recognition - Recognition of courses from the
								study abroad
							</option>
							<option value="37">
								state exam - Postponement of the Final State
								Examination until the end of August
							</option>
							<option value="14">
								state exam - Repeating of the Final State
								Examination in August
							</option>
							<option value="15">
								state exam - Repeating of the oral part of the
								Final State Examination in August
							</option>
							<option value="11">
								state exam - Cancellation of the Final State
								Examination
							</option>
							<option value="32">
								study - Increase of a credit ceiling for the
								study
							</option>
							<option value="33">
								study - Increase of the annual credit ceiling
							</option>
							<option value="34">
								study - Increase of a year's credit ceiling for
								the presumed last year of study
							</option>
							<option value="75">
								study - Promotion to a higher year of study
							</option>
							<option value="9">
								study - Extending the interruption of studies
							</option>
							<option value="45">
								study - Extending the interruption of studies to
								the repetition of the Final State Examination
							</option>
							<option value="41">
								study - Interruption of studies
							</option>
							<option value="44">
								study - Interruption of studies due to the
								repetition of the Final State Examination
							</option>
							<option value="43">
								study - Procedure on Other Student Requests
							</option>
							<option value="8">
								study - Reducing the annual credit minimum
							</option>
							<option value="26">
								study - Exception for continuing studies with a
								lack of credits
							</option>
							<option value="18">
								study - Exception for prolongation of the
								maximum period of study
							</option>
							<option value="7">
								study - Enrolment for studying after the break
								in studies
							</option>
							<option value="35">
								thesis - Postponement of the thesis submission
								and accession to the FSE through a regular term
							</option>
							<option value="36">
								thesis - Postponement of thesis submission until
								the end of July and of the FSE till the end of
								August
							</option>
							<option value="48">
								thesis - Postponement of public access to the
								thesis
							</option>
							<option value="16">
								thesis - Repeating the defence of the thesis in
								August
							</option>
							<option value="49">
								thesis - Allowing of additional issue of the
								assignment of the Bachelor's/Diploma thesis
							</option>
							<option value="51">
								thesis - Permission to adjust the thesis
								assignment
							</option>
							<option value="3">
								thesis - Allowing of change of advisor (and
								assignment) of the Bachelor's thesis/Diploma
								thesis
							</option>
						</select>

						<textarea rows="10" />

						<button type="send" className="btn btn--primary">Send</button>
					</form>
				</div>
			</PageLayout>
		)
	}
}

export default OthersAdministrativeProceduresCreate
