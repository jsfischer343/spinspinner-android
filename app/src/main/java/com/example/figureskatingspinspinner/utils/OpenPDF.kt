package com.example.figureskatingspinspinner.utils

import android.content.ActivityNotFoundException
import android.content.Context
import android.content.Intent
import android.net.Uri
import android.util.Log
import android.widget.Toast
import androidx.core.content.FileProvider
import androidx.core.net.toUri
import java.io.File

fun openPDF(context: Context, pdfUri: Uri) {
    try {
        val intent = Intent(Intent.ACTION_VIEW).apply {
            setDataAndType(pdfUri, "application/pdf")
            addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION)
        }
        try {context.startActivity(intent)} catch(e: ActivityNotFoundException) {
            Toast.makeText(context, "Unable to open PDF viewer", Toast.LENGTH_SHORT).show()
        }

    } catch (e: Exception) {
        Log.e("PDF","Error opening PDF",e)
    }
}

fun getPdfUriFromAssets(context: Context, fileName: String): Uri {
    val file = File(context.filesDir, fileName)
    file.parentFile?.mkdirs()
    if (!file.exists()) {
        context.assets.open(fileName).use { input ->
            file.outputStream().use { output ->
                input.copyTo(output)
            }
        }
    }
    return FileProvider.getUriForFile(
        context,
        "${context.packageName}.fileprovider",
        file
    )
}