export default {
	// Disable server-side rendering: https://go.nuxtjs.dev/ssr-mode
	ssr: false,
	// Target: https://go.nuxtjs.dev/config-target
	target: 'static',
	// Server
	server: {
		host: '0.0.0.0'
	},
	// Environment variables
	env: {
		wsUrl: process.env.WS_HOST || 'ws://localhost:3001/'
	},
	// Global page headers: https://go.nuxtjs.dev/config-head
	head: {
		title: 'Watch Winder Dashboard',
		meta: [
			{ charset: 'utf-8' },
			{ name: 'viewport', content: 'width=device-width, initial-scale=1' },
			{ hid: 'description', name: 'description', content: '' }
		],
		link: [
			{ rel: 'icon', href: '/favicon.ico', type: 'image/x-icon' },
			{ rel: 'preconnect', href: 'https://fonts.googleapis.com' },
			{ rel: 'preconnect', href: 'https://fonts.gstatic.com', crossorigin: true },
			{ rel: 'stylesheet', href: 'https://fonts.googleapis.com/css2?family=Montserrat:wght@400;600;700&display=swap' },
			{ rel: 'stylesheet', href: 'https://fonts.googleapis.com/css2?family=Roboto&display=swap' },
		],
		script: [
			{ src: 'https://polyfill.io/v3/polyfill.min.js?features=es6', body: true },
		]
	},
	// Global CSS: https://go.nuxtjs.dev/config-css
	css: [
		'~/assets/scss/style.scss',
	],
	// Plugins to run before rendering page: https://go.nuxtjs.dev/config-plugins
	plugins: [
		{ src: '~/plugins/bootstrap.client.js' },
		{ src: '~/plugins/lazysizes.client.js' }
	],
	// Auto import components: https://go.nuxtjs.dev/config-components
	components: true,
	// Modules: https://go.nuxtjs.dev/config-modules
	modules: [
		// https://go.nuxtjs.dev/pwa
		'@nuxtjs/pwa',
		// https://i18n.nuxtjs.org/
		'@nuxtjs/i18n',
	],
	// Modules for dev and build (recommended): https://go.nuxtjs.dev/config-modules
	buildModules: [

	],
	// PWA module configuration: https://go.nuxtjs.dev/pwa
	pwa: {
		manifest: {
			lang: 'en'
		}
	},

	// I18n module configuration: https://go.nuxtjs.dev/config-i18n
	i18n: {
		defaultLocale: 'en',
		seo: false,
		langDir: '~/locales/',
		locales: [
			{
				code: 'en',
				iso: 'en-EN',
				name: 'English',
				file: 'en-EN.js'
			},
			{
				code: 'es',
				iso: 'es-ES',
				name: 'Español',
				file: 'es-ES.js'
			}
		],
		vueI18n: {
			fallbackLocale: 'en',
			silentFallbackWarn: true,
			silentTranslationWarn: true,
		}
	},

	// Build Configuration: https://go.nuxtjs.dev/config-build
	build: {
		loaders: {
			vue: {
				compilerOptions: {
					whitespace: 'condense',
					preserveWhitespace: false
				}
			},
			sass: {
				implementation: require('sass')
			},
			scss: {
				implementation: require('sass')
			}
		}
	}
}
