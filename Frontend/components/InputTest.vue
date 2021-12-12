<template>
    <form @submit.prevent="change">
      <div class="row row-cols-auto">
        <div class="col mb-3">
          <label class="form-label" for="type">Type</label>
          <input class="form-control" type="text" id="type" v-model="type" />
        </div>
        <template v-if="type.toLowerCase() == 'action'">
          <div class="col mb-3" style="min-width:300px">
            <label class="form-label" for="action">Action</label>
            <select class="form-select" name="action" id="action" aria-label="Select an action" v-model="action">
              <template v-for="item in arrayActions">
                <option :key="item" :value="item">
                  {{ item }}
                </option>
              </template>
            </select>
          </div>
        </template>
        <template v-if="type.toLowerCase() == 'action' && action.toLowerCase() == 'update'">
          <div class="col-12"></div>
          <div class="col mb-3">
            <label class="form-label" for="setting">Setting</label>
            <select class="form-select" name="setting" id="setting" aria-label="Select an setting" v-model="setting">
              <template v-for="item in arraySettings">
                <option :key="item" :value="item">
                  {{ item }}
                </option>
              </template>
            </select>
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
        arrayActions: ['update', 'restart', 'sleep'],
        settings: {
          controlMode: ['automatic', 'manual'],
          motorStatus: [true, false],
          motorSpeed: null,
          motorDirection: ['both', 'cw', 'ccw'],
          turnsCycles: null,
          turnsPerDay: null,
          turnsPerHour: null,

        }
      }
    },
    computed: {
      arraySettings() {
        return Object.keys(this.settings)
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