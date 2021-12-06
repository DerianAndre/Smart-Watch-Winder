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
						slug: 'connection',
						icon: 'link',
						class: this.settings.connection ? 'bg-success border-success text-light' : 'bg-danger border-danger text-light',
						text: this.settings.connection ? 'On' : 'Off',
					}, {
						slug: 'status',
						icon: 'power',
						class: this.settings.status ? 'bg-success border-success text-light' : 'bg-danger border-danger text-light',
						text: this.settings.status ? 'On' : 'Off',
					}, {
						slug: 'mode',
						icon: this.settings.mode === 'automatic' ? 'gear' : 'sliders',
						text: this.$i18n.t(`settings.modes.${this.settings.mode}`),
					}, {
						slug: 'direction',
						icon: this.iconDirection(),
						text: this.$i18n.t(`settings.directions.${this.settings.direction}`),
					}, {
						slug: 'cycles',
						icon: 'recycle',
						text: this.settings.cycles,
					}, {
						slug: 'turnsSpeed',
						icon: 'speedometer2',
						text: this.settings.turnsSpeed,
					}, {
						slug: 'turnsPerDay',
						icon: 'calendar',
						text: this.settings.turnsPerDay,
					}, {
						slug: 'turnsPerHour',
						icon: 'clock',
						text: this.settings.turnsPerHour,
					}
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
				return types[this.settings.direction];
			}
		}
	}
</script>