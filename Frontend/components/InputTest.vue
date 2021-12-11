<template>
    <form @submit="change">
      <div class="row row-cols-auto">
        <div class="col mb-3">
          <label class="form-label" for="setting">Type</label>
          <input class="form-control" type="text" id="type" v-model="type" />
        </div>
        <template v-if="type.toLowerCase() == 'action'">
          <div class="col mb-3">
            <label class="form-label" for="setting">Action</label>
            <input class="form-control" type="text" id="action" v-model="action" />
          </div>
        </template>
        <template v-if="type.toLowerCase() == 'action' && action.toLowerCase() == 'update'">
          <div class="col-12"></div>
          <div class="col mb-3">
            <label class="form-label" for="setting">Setting</label>
            <input class="form-control" type="text" id="setting" v-model="setting" />
          </div>
          <div class="col mb-3">
            <label class="form-label" for="value">Value</label>
            <input class="form-control" type="text" id="value" v-model="value" />
          </div>
        </template>
      </div>
      <button type="submit" class="btn btn-primary mb-3 px-4" @click="change">
        Send {{ type }}
      </button>
    </form>
</template>

<script>
  export default {
    data() {
      return {
        dataSent: false,
        type: 'action',
        action: 'update',
        setting: 'motorStatus',
        value: 'true',
      }
    },
    methods: {
			change() {
        if(this.dataSent) return;
				let socket = new WebSocket(process.env.wsUrl);
				let $this = this;
				let data = {
					type:    this.type,
					action:  this.action,
					setting: this.action.toLowerCase() == 'update' ? this.setting : null,
					value:   this.action.toLowerCase() == 'update' ? this.value : null,
				}
				socket.onopen = function(e) {
					socket.send(JSON.stringify(data));
          $this.dataSent = true;
          setTimeout(() => {
            $this.dataSent = false;
          }, 500);
				}
			},
		},
  }
</script>