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
using Android.Bluetooth;

namespace K9Control4
{
    class K9Bluetooth
    {
        public bool Connect(Android.Content.Context context,Button btConnect, out BluetoothSocket socket, out string message)
        {
            message = string.Empty;
            if (btConnect != null)
            {
                btConnect.Text = "Connecting..";

            }
            socket = null;
            //BluetoothDevice device = null;
            string result = string.Empty;
            try
            {
                Java.Util.UUID id = Java.Util.UUID.FromString("00001101-0000-1000-8000-00805f9b34fb");

                BluetoothAdapter adapter = BluetoothAdapter.DefaultAdapter;
                adapter.StartDiscovery();
                //BluetoothManager btm = (BluetoothManager)context.GetSystemService(Context.BluetoothService);

                if (adapter.BondedDevices.Count(x => x.Name == "HC-06") == 0)
                {
                    adapter = null;
                    message = "HC-06 device is not paired";
                    btConnect.Text = "Connect";
                    return false;
                }

                BluetoothDevice k9BTdevice = adapter.BondedDevices.First(x => x.Name == "HC-06");
                socket = k9BTdevice.CreateRfcommSocketToServiceRecord(id);

                try
                {
                    k9BTdevice.SetPairingConfirmation(false);
                    //   k9BTdevice.Dispose();
                    k9BTdevice.SetPairingConfirmation(true);
                    btConnect.Text = "Connect";
                }
                catch (Exception ex) { };
                k9BTdevice.CreateBond();
                //context.ApplicationContext.
                socket.Connect();
                btConnect.Text = "Connected";

            }
            catch (Exception ex)
            {
                message = ex.Message;
                return false;
            }

            return true;
        }
    }
}