<template>
    <div>
      <div class="mb-3">
        <label for="setting">Setting</label>
        <input class="form-control" type="text" id="setting" v-model="setting" />
      </div>
      <div class="mb-3">
        <label for="value">Value</label>
        <input class="form-control" type="text" id="value" v-model="value" />
      </div>
      <button class="btn btn-primary mb-3 px-4" @click="change">
        Change
      </button>
    </div>
</template>

<script>
  export default {
    data() {
      return {
        setting: 'direction',
        value: 'ccw',
      }
    },
    methods: {
			change() {
				let $this = this;
				let data = {
					type: "action",
					action: "update",
					setting: this.setting,
					value: this.value,
				}
				let socket = new WebSocket(process.env.wsUrl);
				socket.onopen = function(e) {
					socket.send(JSON.stringify(data));
				}
			},
		},
  }
</script>