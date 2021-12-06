<template>
	<footer class="footer py-3 mt-auto">
		<div class="container d-flex justify-content-between">
			<span>V{{ version }}</span>
			<a href="https://derianandre.com/" class="text-muted small">DerianAndre</a>
		</div>
	</footer>
</template>

<script>
	const { version } = require('../package.json');
	export default {
		data() {
			return {
				version: version
			}
		},
	}
</script>