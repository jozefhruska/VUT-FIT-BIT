import React, { Component } from 'react';
import '../index.scss';

class WinterSemesterCourses extends Component {
	render() {
		return (
			<div className="show-semester-courses">
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
						</tr>

						<tr>
							<td>IAL</td>
							<td>C</td>
							<td>5</td>
							<td>Algorithms</td>
							<td>Cr+Ex</td>
							<td>FIT</td>
							<td>600</td>
							<td>455</td>
						</tr>

						<tr>
							<td>INP</td>
							<td>C</td>
							<td>6</td>
							<td>Design of Computer Systems</td>
							<td>Cr+Ex</td>
							<td>FIT</td>
							<td>600</td>
							<td>447</td>
						</tr>

						<tr>
							<td>IFJ</td>
							<td>C</td>
							<td>5</td>
							<td>Formal Languages and Compilers</td>
							<td>Cr+Ex</td>
							<td>FIT</td>
							<td>600</td>
							<td>466</td>
						</tr>

						<tr>
							<td>INM</td>
							<td>C</td>
							<td>5</td>
							<td>Numerical Methods and Probability</td>
							<td>Cr+Ex</td>
							<td>FEKT</td>
							<td>600</td>
							<td>510</td>
						</tr>

						<tr>
							<td>ISS</td>
							<td>C</td>
							<td>6</td>
							<td>Signals and Systems</td>
							<td>Ex</td>
							<td>FIT</td>
							<td>600</td>
							<td>441</td>
						</tr>

						<tr>
							<td>ITU</td>
							<td>C</td>
							<td>5</td>
							<td>User Interface Programming - 3. year</td>
							<td>ClCr</td>
							<td>FIT</td>
							<td>500</td>
							<td>344</td>
						</tr>						
					</tbody>
				</table>
			</div>
		);
	}
}

export default WinterSemesterCourses;