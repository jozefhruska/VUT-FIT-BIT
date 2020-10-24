import React, { Component } from 'react';
import $ from 'jquery';

import './index.scss';
import PageLayout from '../../components/layout';

class TimetableEdit extends Component{
	componentDidMount(){
		$('table tbody tr td').click(function(){
			if(typeof $(this).attr('class') != "undefined"){
				$(this).toggleClass("toggled");
			}
		});
	}

	render(){
		return(
			<PageLayout
				title="Edit timetable"
			>
				<div className="timetable-edit">
					<table>
						<tbody>
							<tr className="header">
								<td></td>
								<td>8:00</td>
								<td>9:00</td>
								<td>10:00</td>
								<td>11:00</td>
								<td>12:00</td>
								<td>13:00</td>
								<td>14:00</td>
								<td>15:00</td>
								<td>16:00</td>
								<td>17:00</td>
								<td>18:00</td>
								<td>19:00</td>
							</tr>

							<tr>
								<td rowSpan="3" className="filled_bg_header">Mon</td>
								<td colSpan="2" className="filled_bg_practice toggled">ITU</td>
								<td></td>
								<td colSpan="2" className="filled_bg_lecture">INM</td>
								<td colSpan="2"></td>
								<td colSpan="2" className="filled_bg_practice">ITU</td>	
								<td colSpan="2" className="filled_bg_lecture">ISS</td>
								<td></td>
							</tr>

							<tr>
								<td></td>
								<td colSpan="2" className="filled_bg_practice">ITU</td>
								<td colSpan="9"></td>
							</tr>
							<tr>
								<td colSpan="2"></td>
								<td colSpan="2" className="filled_bg_practice">ITU</td>
								<td colSpan="8"></td>
							</tr>

							<tr>
								<td rowSpan="4" className="filled_bg_header">Tue</td>
								<td colSpan="3" className="filled_bg_lecture toggled">ISS</td>
								<td colSpan="3" className="filled_bg_lecture toggled">IAL</td>
								<td colSpan="2"></td>
								<td colSpan="3" className="filled_bg_lecture">IFJ</td>
								<td></td>
							</tr>

							<tr>
								<td colSpan="2" className="filled_bg_practice">ISS</td>
								<td colSpan="3"></td>
								<td colSpan="2" className="filled_bg_practice">ISS</td>	
								<td colSpan="5"></td>
							</tr>

							<tr>
								<td colSpan="2"></td>
								<td colSpan="2" className="filled_bg_practice">IAL</td>
								<td colSpan="8"></td>
							</tr>
							<tr>
								<td colSpan="6"></td>
								<td colSpan="2" className="filled_bg_practice">ISS</td>
								<td colSpan="4"></td>
							</tr>

							<tr>
								<td rowSpan="2" className="filled_bg_header">Wed</td>
								<td colSpan="3" className="filled_bg_lecture">IFJ</td>
								<td colSpan="3" className="filled_bg_lecture toggled">IFJ</td>
								<td colSpan="3" className="filled_bg_lecture">INM</td>
								<td colSpan="3"></td>
							</tr>

							<tr>
								<td colSpan="4"></td>
								<td colSpan="2" className="filled_bg_practice_lecture toggled">INP</td>
								<td colSpan="2" className="filled_bg_practice_lecture">INP</td>
								<td colSpan="4"></td>
							</tr>

							<tr>
								<td rowSpan="4" className="filled_bg_header">Thu</td>
								<td colSpan="3" className="filled_bg_lecture">INP</td>
								<td colSpan="3" className="filled_bg_lecture">INP</td>
								<td colSpan="2"></td>
								<td colSpan="3" className="filled_bg_lecture">IFJ</td>
								<td></td>
							</tr>

							<tr>
								<td colSpan="2" className="filled_bg_practice">IFJ</td>
								<td colSpan="3"></td>
								<td colSpan="2" className="filled_bg_practice">ISS</td>	
								<td colSpan="5"></td>
							</tr>

							<tr>
								<td colSpan="2"></td>
								<td colSpan="2" className="filled_bg_practice">IAL</td>
								<td colSpan="8"></td>
							</tr>
							<tr>
								<td colSpan="6"></td>
								<td colSpan="2" className="filled_bg_practice">ITU</td>
								<td colSpan="4"></td>
							</tr>
						</tbody>

						<div className="legend">
							<p><span className="label label-choosen" /> Chosen</p>
							<p><span className="label label-lecture" /> Lecture</p>
							<p><span className="label label-practice" /> Practice</p>
							<p><span className="label label-practice-lecture" /> Practice lecture</p>
						</div>
					</table>

					<div className="timetable-edit-save-button">
						<button className="btn btn--primary">Save</button>
					</div>
				</div>
			</PageLayout>
		);
	}
}

export default TimetableEdit;