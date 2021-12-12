<template>
	<div class="container">
		<div class="card shadow-lg mb-4">
			<div class="card-body py-5 px-4">
				<h1 class="h2">
					About
				</h1>
				<p class="lead mb-4">
					Another open-source handcrafted project made with 🧠 by <a href="https://github.com/DerianAndre">Derian André</a>
				</p>
				<a class="btn btn-dark px-4" href="https://github.com/DerianAndre/Smart-Watch-Winder">
					<i class="bi bi-github me-2" />
					Github Repository
				</a>
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
					status: false,
					mode: 'automatic',
					direction: 'cw',
					tpd: 640,
					tph: 25,
					cycles: 24,
					sct: 1,
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
				this.settings.cycles = item.cycles || 24;
				this.settings.tpd = item.tpd.split('-') ? item.tpd.split('-')[0] : item.tpd;
				this.settings.tph = this.settings.tpd / this.settings.cycles;
				this.settings.direction = item.direction;
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