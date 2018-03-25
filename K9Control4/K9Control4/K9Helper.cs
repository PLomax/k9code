using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

using Android.App;
using Android.Content;
using Android.OS;
using Android.Runtime;
using Android.Views;
using Android.Widget;



namespace K9Control4
{
    public static class K9Helper
    {
        public static void dialogOKBox(string message, Android.Content.Context context)
        {
            var callDialog = new AlertDialog.Builder(context);
            callDialog.SetMessage("Bluetooth:\n" + message);
            callDialog.SetNeutralButton("OK", delegate { });
            callDialog.Show();

        }

    }
}

    