<template>
	<button class="d-flex align-items-center btn rounded-pill px-4" :class="settings.status ? 'btn-success' : 'btn-danger'" @click="toggleStatus()">
		<i class="bi" :class="settings.status ? 'bi-toggle2-on' : 'bi-toggle2-off'" />
		<span class="d-none d-md-block ms-2 small">
			{{ settings.status ? 'ON' : 'OFF' }}
		</span>
	</button>
</template>

<script>
	export default {
		data() {
			return {
				settings: {
					status: false,
				},
			}
		},
		methods: {
			toggleStatus() {
				let $this = this;
				let data = {
					type: "action",
					action: "update",
					setting: "status",
					value: !this.settings.status,
				}
				let socket = new WebSocket(process.env.wsUrl);
				socket.onopen = function(e) {
					socket.send(JSON.stringify(data));
					$this.settings.status = !$this.settings.status;
				}
			},
		},
	}
</script>