<template>
	<button class="d-flex align-items-center btn rounded-pill px-4" :class="settings.motorStatus ? 'btn-success' : 'btn-danger'" @click="toggleStatus()" :disabled="!settings.connection">
		<i class="bi" :class="settings.motorStatus ? 'bi-toggle2-on' : 'bi-toggle2-off'" />
		<span class="d-none d-md-block ms-2 small">
			{{ settings.motorStatus ? 'ON' : 'OFF' }}
		</span>
	</button>
</template>

<script>
	export default {
		props: {
			settings: {
				type: Object,
				required: true,
			},
		},
		methods: {
			toggleStatus() {
				let $this = this;
				let data = {
					type: "action",
					action: "update",
					setting: "motorStatus",
					value: !this.settings.motorStatus,
				}
				let socket = new WebSocket(process.env.wsUrl);
				socket.onopen = function(e) {
					socket.send(JSON.stringify(data));
					$this.settings.motorStatus = !$this.settings.motorStatus;
				}
			},
		},
	}
</script>