import React, { Component } from 'react';
import '../index.scss';

class WinterSemesterCourses extends Component {
	render() {
		return (
			<div className="semester-courses">
				<table className="hug">
					<tbody>
						<tr className="header">
							<td>Abbr</td>
							<td>Type</td>
							<td>Cr</td>
							<td>Title</td>
							<td>Compl</td>
							<td>Fa</td>
							<td>Max</td>
							<td>Count</td>
							<td>Reg.</td>
						</tr>

						<tr className="table-compulsory">
							<td>IAL</td>
							<td>C</td>
							<td>5</td>
							<td>Algorithms</td>
							<td>Cr+Ex</td>
							<td>FIT</td>
							<td>600</td>
							<td>455</td>
							<td><input type="checkbox" defaultChecked /></td>
						</tr>

						<tr className="table-compulsory">
							<td>INP</td>
							<td>C</td>
							<td>6</td>
							<td>Design of Computer Systems</td>
							<td>Cr+Ex</td>
							<td>FIT</td>
							<td>600</td>
							<td>447</td>
							<td><input type="checkbox" defaultChecked /></td>
						</tr>

						<tr className="table-compulsory">
							<td>IFJ</td>
							<td>C</td>
							<td>5</td>
							<td>Formal Languages and Compilers</td>
							<td>Cr+Ex</td>
							<td>FIT</td>
							<td>600</td>
							<td>466</td>
							<td><input type="checkbox" defaultChecked /></td>
						</tr>

						<tr className="table-compulsory">
							<td>INM</td>
							<td>C</td>
							<td>5</td>
							<td>Numerical Methods and Probability</td>
							<td>Cr+Ex</td>
							<td>FEKT</td>
							<td>600</td>
							<td>510</td>
							<td><input type="checkbox" defaultChecked /></td>
						</tr>

						<tr className="table-compulsory">
							<td>ISS</td>
							<td>C</td>
							<td>6</td>
							<td>Signals and Systems</td>
							<td>Ex</td>
							<td>FIT</td>
							<td>600</td>
							<td>441</td>
							<td><input type="checkbox" defaultChecked /></td>
						</tr>

						<tr className="table-compulsory">
							<td>ITU</td>
							<td>C</td>
							<td>5</td>
							<td>User Interface Programming - 3. year</td>
							<td>ClCr</td>
							<td>FIT</td>
							<td>500</td>
							<td>344</td>
							<td><input type="checkbox" defaultChecked /></td>
						</tr>

						
						<tr className="table-volatile">
							<td>IPA</td>
							<td>E</td>
							<td>5</td>
							<td>Advanced Assembly Languages</td>
							<td>Ex</td>
							<td>FIT</td>
							<td>90</td>
							<td>2</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>ILI</td>
							<td>E</td>
							<td>4</td>
							<td>Advanced Topics of Linux Administration</td>
							<td>ClCr</td>
							<td>FIT</td>
							<td>40</td>
							<td>4</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>IACH</td>
							<td>E</td>
							<td>3</td>
							<td>Architecture of the 20th Century (hum.)</td>
							<td>Ex</td>
							<td>FAVU</td>
							<td>5</td>
							<td>2</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>HKA</td>
							<td>E</td>
							<td>3</td>
							<td>Assertiveness and Conflicts (hum.)</td>
							<td>Cr</td>
							<td>FIT</td>
							<td>20</td>
							<td>1</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>IPMA</td>
							<td>E</td>
							<td>5</td>
							<td>Business Management</td>
							<td>Cr+Ex</td>
							<td>FEKT</td>
							<td>25</td>
							<td>1</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>HKO</td>
							<td>E</td>
							<td>3</td>
							<td>Communication Skills (hum.)</td>
							<td>Cr</td>
							<td>FIT</td>
							<td>20</td>
							<td>1</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>JA3</td>
							<td>E</td>
							<td>3</td>
							<td>Conversation through Hot Current Issues</td>
							<td>Cr+Ex</td>
							<td>FEKT</td>
							<td>30</td>
							<td>2</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>IKPT</td>
							<td>E</td>
							<td>5</td>
							<td>Culture of Speech and the Generation of Texts</td>
							<td>Cr</td>
							<td>FEKT</td>
							<td>10</td>
							<td></td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>ICUZ</td>
							<td>E</td>
							<td>3</td>
							<td>Czech Art of the First Half of the 20th Century in Context (hum.)</td>
							<td>Ex</td>
							<td>FAVU</td>
							<td>5</td>
							<td></td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>IPSO</td>
							<td>E</td>
							<td>5</td>
							<td>Educational Psychology</td>
							<td>Ex</td>
							<td>FEKT</td>
							<td>30</td>
							<td>1</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>IIPD</td>
							<td>E</td>
							<td>5</td>
							<td>Engineering Pedagogy and Didactics</td>
							<td>Ex</td>
							<td>FEKT</td>
							<td>30</td>
							<td></td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>AEU</td>
							<td>E</td>
							<td>3</td>
							<td>English for Europe</td>
							<td>Cr+Ex</td>
							<td>FEKT</td>
							<td>20</td>
							<td>1</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>FCE</td>
							<td>E</td>
							<td>3</td>
							<td>English: Preparatory Course for FCE Exam</td>
							<td>Cr</td>
							<td>FIT</td>
							<td>50</td>
							<td>4</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>PRM</td>
							<td>E</td>
							<td>3</td>
							<td>Fundamentals of Law (hum.)</td>
							<td>Cr</td>
							<td>ICV</td>
							<td>80</td>
							<td>8</td>
							<td><input type="checkbox" /></td>
						</tr>
						
						<tr className="table-volatile">
							<td>FIT</td>
							<td>E</td>
							<td>3</td>
							<td>History and Philosophy of Technology (hum.)</td>
							<td>Cr</td>
							<td>ICV</td>
							<td>80</td>
							<td>6</td>
							<td><input type="checkbox" /></td>
						</tr>
					</tbody>
				</table>
			</div>
		);
	}
}

export default WinterSemesterCourses;