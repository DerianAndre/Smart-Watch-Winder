<template>
	<div class="item border">
		<i :class="iconClass" />
		<div class="info">
			<b>{{ setting }}</b>
			{{ text }}
		</div>
	</div>
</template>

<script>
	export default {
		props: ['icon', 'setting', 'text'],
		computed: {
			iconClass() {
				return `bi bi-${ this.icon }`;
			}
		}
	}
</script>

<style lang="scss">
	.item {
		padding: 0.75rem 1.5rem;
		border-radius: 0.5rem;
		display: flex;
		align-items: center;
		.bi {
			font-size: 1.75rem;
			margin-right: 1rem;
		}
		.info {
			display: flex;
			flex-direction: column;
			b {
				text-transform: uppercase;
				font-size: 80%;
			}
		}
	}
</style>