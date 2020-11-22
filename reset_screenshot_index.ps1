# Get screenshot index
If($null -ne ($screenshot_index = (Get-ItemProperty -Path "HKCU:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer" -Name "ScreenshotIndex" -ErrorAction SilentlyContinue).ScreenshotIndex)){
	If($screenshot_index -eq 0){
		Write-Host -Object "Screenshot index has already been reset!"
	}
	Else{

		# Reset screenshot index
		Set-ItemProperty -Path "HKCU:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer" -Name "ScreenshotIndex" -Value 0 -ErrorAction SilentlyContinue
		If((Get-ItemProperty -Path "HKCU:\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Explorer" -Name "ScreenshotIndex" -ErrorAction SilentlyContinue).ScreenshotIndex -eq 0){
			Write-Host -Object "Screenshot index is successfully reset from ${screenshot_index}!"
		}
		Else{
			Write-Host -Object "An error occurved while attempting to reset the screenshot index..."
		}
	}
}
Else{
	Write-Host -Object "An error occurved while attempting to get the screenshot index..."
}