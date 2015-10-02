PropertySheet Naming Convention

Global_Base:
	Set global include path
	Set lib path if not configuration-dependent

Global_Debug :
Global_Release:
	Set configuration-dependent lib path
	Set configuration-dependent lib names

($ProjName)_Base:
	Set include path(and lib path if not dependent on configuration) 
	Set lib path if not configuration-dependent

($ProjName)_Debug:
($ProjName)_Release:
	Set configuration-dependent lib path
	Set configuration-dependent lib names
	