<template>
	<div class="container">
		<div class="card shadow-lg mb-4">
			<div class="card-body py-5 px-4">
				<h1 class="h2">
					Overview
				</h1>
				<p class="lead mb-4">
					This is a simple overview of the Watch Winder. See all your settings in a glance.
				</p>
				{{ settings.connection }}
				<DashboardGrid class="mb-4" :settings="settings" />
				
				<h4 class="mb-3">Quick actions</h4>
				<div class="hstack gap-3 mb-4">
					<PowerToggle class="btn-lg px-5" />
				</div>
				<h4 class="mb-3">Manual action</h4>
				<InputTest />
			</div>
		</div>
	</div>
</template>

<script>
	export default {
		data() {
			return {
				settings: {
					connection: false,
					status: false,
					mode: 'automatic',
					direction: 'cw',
					turnsSpeed: 1,
					turnsPerSecond: 60,
					turnsPerHour: 25,
					turnsPerDay: 640,
					cycles: 24,
					cyclesSleepTime: 1,
				}
			}
		},
		mounted() {
			let $this = this;
			let ws = new WebSocket(process.env.wsUrl);

			ws.onopen = function(e) {
				$this.settings.connection = true;
				let data = {
					type: "id",
					client: {
						id: "SWWF1231251243",
						type: "frontend",
					}
				}
				ws.send(JSON.stringify(data));
			};

			ws.onclose = function(e) {
				$this.settings.connection = false;
			};
			
			ws.onmessage = function(e) {
				let message = JSON.parse(e.data);
				if(message.type == 'action') {
					console.log(message);
				}
			};
		},
	}
</script>