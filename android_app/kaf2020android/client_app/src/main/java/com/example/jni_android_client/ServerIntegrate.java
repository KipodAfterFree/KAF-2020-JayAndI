package com.example.jni_android_client;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;
import java.util.Objects;
import java.util.concurrent.atomic.AtomicReference;

import okhttp3.HttpUrl;
import okhttp3.MediaType;
import okhttp3.MultipartBody;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class ServerIntegrate {
    final String server_url = "http://10.0.0.2:12345";
            // "https://jayandi.ctf.kaf.sh"; // "http://10.0.0.1:12345";

    /* access modifiers changed from: package-private */
    public Response sendRequest(String base_url, String endpoint, Map<String, String> params) throws IOException {
        OkHttpClient client = new OkHttpClient();
        try {
            HttpUrl.Builder urlBuilder = ((HttpUrl) Objects.requireNonNull(HttpUrl.parse(base_url + endpoint))).newBuilder();
            for (Map.Entry<String, String> entry : params.entrySet()) {
                urlBuilder.addQueryParameter(entry.getKey(), entry.getValue());
            }
            return client.newCall(new Request.Builder().header("Authorization", "Basic S2lwb2Q6a2lwb2RhZnRlcmZyMzM=").url(urlBuilder.build().toString()).build()).execute();
        } catch (NullPointerException e) {
            return null;
        }
    }

    public Response sendPostRequest(String base_url, String endpoint, Map<String, String> params) throws IOException {
        OkHttpClient client = new OkHttpClient();
        try {
            StringBuilder str_params = new StringBuilder();
            for (Map.Entry<String, String> entry : params.entrySet()) {
                str_params.append(entry.getKey()).append("=").append(entry.getValue()).append("&");
            }
            str_params.deleteCharAt(str_params.length() - 1);
            RequestBody body = RequestBody.create(params.toString(), MediaType.get("text/plain"));

            return client.newCall(new Request.Builder().post(body).url(base_url + endpoint).addHeader("Authorization", "Basic S2lwb2Q6a2lwb2RhZnRlcmZyMzM=").build()).execute();
        } catch (NullPointerException e) {
            return null;
        }
    }

    public Response sendImagePostRequest(String base_url, String endpoint, Map<String, String> params) throws IOException {
        OkHttpClient client = new OkHttpClient();
        try {
            MultipartBody.Builder builder = new MultipartBody.Builder();
            builder.setType(MultipartBody.FORM);
            for (Map.Entry<String, String> entry : params.entrySet()) {
                builder.addFormDataPart(entry.getKey(), entry.getValue());
            }
            RequestBody body = builder.build(); // MultipartBody.Builder().addForm RequestBody.create(params.toString(), MediaType.get("text/plain"));
            return client.newCall(new Request.Builder().post(body).url(base_url + endpoint).build()).execute();
        } catch (NullPointerException e) {
            return null;
        }
    }

    /* access modifiers changed from: package-private */
    public Boolean register(String username, String password, String age) throws IOException {
        Map<String, String> params = new HashMap<>();
        params.put("username", username);
        params.put("password", password);
        params.put("age", age);
        Response resp = sendRequest(server_url, "/register", params);
        return resp.code() == 200;
    }

    /* access modifiers changed from: package-private */
    public Boolean login(String username, String password) throws IOException {
        Map<String, String> params = new HashMap<>();
        params.put("username", username);
        params.put("password", password);
        Response resp = sendRequest(server_url, "/login", params);
        return resp.code() == 200;
    }

    public void sendPicture(String username, String password, String target, String image) throws IOException {
        Map<String, String> params = new HashMap<>();
        params.put("username", username);
        params.put("target", target);
        params.put("password", password);
        params.put("image", image);

        Response resp = sendImagePostRequest(server_url, "/send_image", params);
        if (resp.code() != 200)
            return;

        Objects.requireNonNull(resp.body()).string();
    }

    volatile AtomicReference<Response> resp = new AtomicReference<Response>();

    public String getUserList(String username, String password) throws IOException, InterruptedException {

        final Map<String, String> params = new HashMap<>();
        params.put("username", username);
        params.put("password", password);
        Thread th = new Thread(new Runnable() {
            @Override
            public void run() {
                try {
                    resp.set(sendRequest(server_url, "/users", params));
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }
        });


        th.start();
        th.join();

        if (resp.get().code() != 200)
            return null;

        return Objects.requireNonNull(resp.get().body()).string();
    }

    public String getPicture(String username, String password) throws IOException {
        Map<String, String> params = new HashMap<>();
        params.put("username", username);
        params.put("password", password);
        Response resp = sendRequest(server_url, "/recv_image", params);
        if (resp.code() != 200)
            return null;

        return Objects.requireNonNull(resp.body()).string();
    }
}