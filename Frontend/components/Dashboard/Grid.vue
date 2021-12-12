<template>
	<div class="row row-cols-1 row-cols-sm-2 row-cols-md-3 row-cols-xl-4 gx-3">
		<div class="col mb-3" v-for="item in items" :key="item.slug">
			<DashboardItem :class="item.class" :icon="item.icon" :setting="$t(`settings.${item.slug}`)" :text="item.text" />
		</div>
	</div>
</template>

<script>
	export default {
		props: ['settings'],
		data() {
			return {
				items: [
					{
						slug:  'connection',
						icon:  'link',
						class: this.settings.connection ? 'bg-success border-success text-light' : 'bg-danger border-danger text-light',
						text:  this.settings.connection ? 'On':'Off',
					}, {
						slug:  'controlMode',
						icon:  this.settings.controlMode === 'automatic' ? 'gear': 'sliders',
						class: this.settings.controlMode ? (this.settings.controlMode === 'automatic' ? 'bg-primary border-primary text-light' : 'bg-secondary border-secondary text-light') : null,
						text:  this.settings.controlMode ? this.$i18n.t(`settings.modes.${this.settings.controlMode}`) : '-',
					}, {
						slug:  'motorStatus',
						icon:  'power',
						class: this.settings.motorStatus != null ? (this.settings.motorStatus == true ? 'bg-success border-success text-light': 'bg-danger border-danger text-light') : null,
						text:  this.settings.motorStatus != null ? (this.settings.motorStatus == true ? 'On': 'Off') : '-',
					}, {
						slug: 'motorDirection',
						icon: this.iconDirection(),
						text: this.settings.motorDirection ? this.$i18n.t(`settings.directions.${this.settings.motorDirection}`) : '-',
					}, {
						slug: 'motorSpeed',
						icon: 'speedometer2',
						text: this.settings.motorSpeed ? this.settings.motorSpeed : '-',
					}, {
						slug: 'turnsCycles',
						icon: 'recycle',
						text: this.settings.turnsCycles ? this.settings.turnsCycles : '-',
					}, {
						slug: 'turnsPerHour',
						icon: 'clock',
						text: this.settings.turnsPerHour ? this.settings.turnsPerHour : '-',
					}, {
						slug: 'turnsPerDay',
						icon: 'calendar',
						text: this.settings.controlMode ? (this.settings.controlMode === 'automatic' ? this.settings.turnsPerDay : this.settings.turnsCycles * this.settings.turnsPerHour) : '-',
					},
				],
			}
		},
		methods: {
			iconDirection() {
				let types = {
					'cw': 'arrow-clockwise',
					'ccw': 'arrow-counterclockwise',
					'both': 'arrow-repeat',
					'shuffle': 'shuffle',
				};
				return this.settings.motorDirection ? types[this.settings.motorDirection] : 'question-circle';
			}
		}
	}
</script>