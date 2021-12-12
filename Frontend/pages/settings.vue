<template>
	<div class="container">
		<div class="card shadow-lg mb-4">
			<div class="card-body py-5 px-4">
				<h1 class="h2">
					Settings
				</h1>
				<p class="lead mb-4">
					Control all the settings of your watch winder.
				</p>

				<h3>
					General
				</h3>
				<p>
					Most automatic watches require 500 to 800 TPD although there are others with longer power reserves that need 1300 TPD depending on complications.
				</p>
				
				<section class="mb-4">
					<label for="searchBar" class="form-label">Database search</label>
					<input class="form-control mb-2" type="text" id="searchBar" placeholder="Search for a brand or a model..." v-model="search.query" aria-describedby="searchHelp" @input="searchQuery()"/>
					<div id="searchHelp" class="form-text">
						If you don't know what values to use, search in the database and click the desired model.
					</div>
					
					<div class="table-responsive rounded border border-dark" v-if="search.results.length >0">
						<table class="table table-hover position-relative m-0">
							<thead>
								<tr>
									<th scope="col">Brand</th>
									<th scope="col">Model</th>
									<th scope="col">Movement</th>
									<th scope="col">Direction</th>
									<th scope="col">TPD</th>
								</tr>
							</thead>
							<tbody>
								<tr v-for="item in search.results" v-bind:key="item.model" @click="setData(item)"> 
									<th scope="row">
										{{ item.brand }}
									</th>
									<td>
										{{ item.model }}
									</td>
									<td>
										{{ item.movement }}
									</td>
									<td>
										{{ item.motorDirection }}
									</td>
									<td>
										{{ item.tpd }}
									</td>
								</tr>
							</tbody>
						</table>
					</div>
					<div class="no-search" v-else>
						<p v-if="search.query != ''">
							No results found.
						</p>
					</div>
				</section>
				
				<section class="mb-5">
					<h4>
						Mode
					</h4>
					<div class="btn-group mb-3" role="group" aria-label="Winder Control Mode">
						<input type="radio" class="btn-check" name="controlMode" v-model="settings.controlMode" id="automatic" value="automatic" autocomplete="off">
						<label class="d-flex align-items-center btn btn-outline-dark" for="automatic">
							<i class="fw-bolder bi bi-gear"></i>
							<span class="d-none d-sm-block ms-2 small">Automatic</span>
						</label>

						<input type="radio" class="btn-check" name="controlMode" v-model="settings.controlMode" id="manual" value="manual" autocomplete="off">
						<label class="d-flex align-items-center btn btn-outline-dark" for="manual">
							<i class="fw-bolder bi bi-sliders"></i>
							<span class="d-none d-sm-block ms-2 small">Manual</span>
						</label>
					</div>
					<div>
						<div v-if="settings.controlMode == 'automatic'">
							<h5>
								Automatic settings
							</h5>
							<div class="mb-3">
								<label for="turnsPerDay" class="form-label">Turns per day (TPD)</label>
								<input type="number" class="form-control" id="tpd" aria-describedby="turnsPerDayHelp" v-model="settings.turnsPerDay">
								<div id="turnsPerDayHelp" class="form-text">
									That is the number of turns of the internal rotor that is required each day to keep the particular watch wound.
								</div>
							</div>
						</div>
						<div v-else>
							<h5>
								Manual settings
							</h5>
							<div class="row row-cols-1 row-cols-md-3">
								<div class="col mb-3">
									<label for="turnsPerHour" class="form-label">Turns per hour (TPH)</label>
									<input type="number" class="form-control" id="turnsPerHour" aria-describedby="turnsPerHourHelp" v-model="settings.turnsPerHour">
									<div id="turnsPerHourHelp" class="form-text">
										That is the number of turns of the internal rotor that is required each day to keep the particular watch wound.
									</div>
								</div>
								<div class="col mb-3">
									<label for="turnsCycles" class="form-label">Cycles</label>
									<input type="number" class="form-control" id="turnsCycles" aria-describedby="turnsCyclesHelp" v-model="settings.turnsCycles">
									<div id="turnsCyclesHelp" class="form-text">
										The number of cycles that the winder will do.
									</div>
								</div>
							</div>

							<div class="mb-3">
								<label for="turnsPerDay" class="form-label">Turns per day (TPD)</label>
								<input type="number" class="form-control" id="turnsPerDay" aria-describedby="turnsPerDayHelp" :value="settings.turnsPerHour * settings.turnsCycles" disabled>
								<div id="tpurnsPerDayelp" class="form-text">
									That is the number of turns of the internal rotor that is required each day to keep the particular watch wound.
								</div>
							</div>
						</div>
					</div>
				</section>

				<section class="mb-5">
					<h4>
						Advanced settings
					</h4>
					<div class="mb-3">
						<label for="tpd" class="form-label">Cycles sleep time</label>
						<input type="number" class="form-control" id="cst" v-model="settings.cyclesSleepTime" aria-describedby="cstHelp">
						<div id="cstHelp" class="form-text">
							The time in hours that the winder will sleep after each cycle.
						</div>
					</div>
				</section>

				<section class="mb-5">
					<h4>
						Direction
					</h4>
					<div class="btn-group" role="group" aria-label="Winder motorDirection">
						<input type="radio" class="btn-check" name="motorDirection" v-model="settings.motorDirection" id="both" value="both" autocomplete="off" checked>
						<label class="d-flex align-items-center btn btn-outline-dark" for="both">
							<i class="fw-bolder bi bi-arrow-repeat"></i>
							<span class="d-none d-sm-block ms-2 small">
								Both
							</span>
						</label>

						<input type="radio" class="btn-check" name="motorDirection" v-model="settings.motorDirection" id="cw" value="cw" autocomplete="off">
						<label class="d-flex align-items-center btn btn-outline-dark" for="cw">
							<i class="fw-bolder bi bi-arrow-clockwise"></i>
							<span class="d-none d-sm-block ms-2 small">
								Clockwise
							</span>
						</label>

						<input type="radio" class="btn-check" name="motorDirection" v-model="settings.motorDirection" id="ccw" value="ccw" autocomplete="off">
						<label class="d-flex align-items-center btn btn-outline-dark" for="ccw">
							<i class="fw-bolder bi bi-arrow-counterclockwise"></i>
							<span class="d-none d-sm-block ms-2 small">
								Counter clockwise
							</span>
						</label>
					</div>
				</section>
				<hr>

				<button class="d-flex align-items-center btn btn-success" @click="saveChanges()">
					<i class="fw-bolder bi bi-check2"></i>
					<span class="d-none d-sm-block ms-2 small">
						Save changes
					</span>
				</button>
			</div>
		</div>
	</div>
</template>

<script>
	import database from '../db/0.json';
	export default {
		data() {
			return {
				settings: {
					controlMode: 'automatic',
					motorStatus: false,
					motorDirection: 'cw',
					turnsPerDay: 640,
					turnsPerHour: 25,
					turnsCycles: 24,
					cyclesSleepTime: 1,
				},
				search: {
					database: database,
					query: '',
					results: [],
				}
			}
		},
		methods: {
			searchQuery() {
				if (this.search.query.length == 0 || this.search.query == '') {
					this.search.results = [];
				} else {
					this.search.results = this.search.database.filter(
						item => item.model.toLowerCase().includes(this.search.query.toLowerCase())
					);
				}
			},
			setData(item) {
				this.search.results = [];
				this.search.query = '';
				this.settings.turnsCycles = item.cycles || 24;
				this.settings.turnsPerDay = item.tpd.split('-') ? item.tpd.split('-')[0] : item.tpd;
				this.settings.turnsPerHour = this.settings.turnsPerDay / this.settings.turnsCycles;
				this.settings.motorDirection = item.motorDirection;
			}
		},
	}
</script>

<style lang="scss">
	input {
		max-width: 400px;
	}
	.form-label {
		text-transform: uppercase;
		font-weight: bold;
		font-size: 80%;
	}
	.table-responsive {
		max-height: 300px;
	}
	.table {
		thead {
			tr {
				th {
					font-size: 80%;
					color: white;
					background: #000;
					font-weight: bold;
					text-transform: uppercase;
					border: 0;
					position: sticky;
					top: 0;
				}
			}
		}
		tbody {
			tr {
				font-size: 80%;
				&:hover {
					cursor: pointer;
				}
			}
		}
	}
</style>