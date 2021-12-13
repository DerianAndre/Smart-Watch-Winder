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
				
				<DashboardGrid class="mb-4" :settings="settings" :key="counter"/>
				
				<h4 class="mb-3">Quick actions</h4>
				<div class="hstack gap-3 mb-4">
					<PowerToggle class="btn-lg px-5" :settings="settings"/>
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
				counter: 0,
				settings: {
					connection: false,
					controlMode: null,
					motorStatus: null,
					motorSpeed: null,
					motorSpeedMin: null,
					motorSpeedMax: null,
					motorDirection: null,
					turnsSpeed: null,
					turnsPerSecond: null,
					turnsPerHour: null,
					turnsPerDay: null,
					cycles: null,
					cyclesSleepTime: null,
				}
			}
		},
		mounted() {
			let $this = this;
			let ws = new WebSocket(process.env.wsUrl);

			ws.onopen = function(e) {
				let data = {
					type: "connection",
					id: "SWWF1231251243",
					client: "frontend",
				}
				ws.send(JSON.stringify(data));
			};

			ws.onclose = function(e) {
				$this.settings.connection = false;
			};
			
			ws.onmessage = function(e) {
				let message = JSON.parse(e.data);
				$this.counter++;
				console.log(message);
				if(message.type == 'action') {
					// console.log(message);
				}
				if(message.type == 'settings') {
					$this.settings.connection = true;
					$this.settings.id = message.id;
					$this.settings.controlMode = message.controlMode;
					$this.settings.motorDirection = message.motorDirection;
					$this.settings.motorStatus = message.motorStatus;
					$this.settings.motorSpeed = message.motorSpeed;
					$this.settings.motorSpeedMin = message.motorSpeedMin;
					$this.settings.motorSpeedMax = message.motorSpeedMax;
					$this.settings.turnsCycles = message.turnsCycles;
					$this.settings.turnsPerDay = message.turnsPerDay;
					$this.settings.turnsPerHour = message.turnsPerHour;
				}
			};
		},
	}
</script>